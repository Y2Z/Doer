#include <QTimer>

#include "singleinstance.hpp"

#if defined(Q_OS_WIN)
#include <windows.h>
#elif !defined(Q_OS_MACOS)
#include <X11/Xlib.h>
#endif

SingleInstance::SingleInstance(QWidget *parent, QString *progName) : QObject(parent)
{
    shmemName = progName + QString("_shrdmmr");
    smphorName = progName + QString("_smphr");
}

SingleInstance::~SingleInstance()
{
}

bool SingleInstance::isAlreadyRunning(const bool raiseExisting)
{
    QSystemSemaphore sem(smphorName, 1);

    // Fix danging memory on Linux
    {
        QSharedMemory fix(shmemName);
        fix.attach();
    }

    shMem = new QSharedMemory(shmemName);
    if (shMem->create(sizeof(SharedData))) { // This is the first instance
        shMem->attach();

        sem.acquire();
        memset(shMem->data(), 0, shMem->size());
        sem.release();

        QTimer* t = new QTimer(this);
        connect(t, &QTimer::timeout, this, [this]() {
            QSystemSemaphore sem(smphorName, 1);
            sem.acquire();
            SharedData* data = reinterpret_cast<SharedData*>(shMem->data());
            if (data->needToRaiseExistingWindow) {
                SingleInstance::raiseWindow((QWidget*)parent());
                data->needToRaiseExistingWindow = false;
            }

            sem.release();
        });
        t->start(0);
    } else { // Another instance is already running
        shMem->attach();

        sem.acquire();
        SharedData* data = reinterpret_cast<SharedData*>(shMem->data());
        data->needToRaiseExistingWindow = raiseExisting;

        return true;
    }

    return false;
}

void SingleInstance::raiseWindow(QWidget *window)
{
#if defined(Q_OS_WIN) // Windows
    Window winId = window->effectiveWinId();

    SetWindowPos(winId, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    SetWindowPos(winId, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#elif defined(Q_OS_MACOS) // macOS
    window->show();
    window->raise();
    window->activateWindow();
#else // GNU/Linux, FreeBSD, etc
    Window winId = window->effectiveWinId();

    if (winId > 0) {
        Display* disp = XOpenDisplay(nullptr);

        if (disp) {
            XWindowAttributes attributes;

            if (XGetWindowAttributes(disp, winId, &attributes)) {
                XSetInputFocus(disp, winId, RevertToPointerRoot, CurrentTime);
                XRaiseWindow(disp, winId);
            }

            XCloseDisplay(disp);
        }
    }
#endif
}
