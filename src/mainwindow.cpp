#include <QSettings>
#include <QCloseEvent>
#include <QShortcut>
#include <QTimer>
#include <QMenu>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    trayIcon(QIcon(":/images/tray.png"), this)
{
    ui->setupUi(this);

    loadSettings();

    setMinimumSize(240, 360);

    setIcon();

    setStyle();
    ui->textArea->setFrameStyle(QFrame::NoFrame);

    bindShortcuts();

    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, this,
                     [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            toggleHidden();
        }
    });

    QMenu *menu = new QMenu { this };

    menu->addAction(QString("Quit"), [&]()->void {
        quitApplication();
    });

    trayIcon.setContextMenu(menu);
    trayIcon.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("doer", "doer");

    if (settings.contains("text")) {
        QString content = settings.value("text").toString();

        ui->textArea->setText(content);
        ui->textArea->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }

    if (settings.contains("geometry")) {
        restoreGeometry(
            QByteArray::fromHex(settings.value("geometry").toByteArray())
        );
    }
}

void MainWindow::setIcon()
{
    QIcon windowIcon(":/images/doer.svg");

    setWindowIcon(windowIcon);
}

void MainWindow::setStyle()
{
    QFile file(":/styles/default.qss");

    file.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(file.readAll());

    ui->textArea->setStyleSheet(styleSheet);
}

void MainWindow::bindShortcuts()
{
    // Connect the fullscreen shortcut (Ctrl+F)
    QAction *fullScreenAction = new QAction(this);
    fullScreenAction->setShortcut(QKeySequence("Ctrl+F"));
    addAction(fullScreenAction);
    connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

    // Connect the fullscreen shortcut (F11)
    QAction *fullScreenAction2 = new QAction(this);
    fullScreenAction2->setShortcut(QKeySequence(Qt::Key_F11));
    addAction(fullScreenAction2);
    connect(fullScreenAction2, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

    // Connect the fullscreen exit shortcut (Esc)
    QAction *fullScreenExitAction = new QAction(this);
    fullScreenExitAction->setShortcut(QKeySequence(Qt::Key_Escape));
    addAction(fullScreenExitAction);
    connect(fullScreenExitAction, SIGNAL(triggered()), this, SLOT(exitFullScreenOrHide()));

    // Connect the quit shortcut (Ctrl+Q)
    QAction *quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence("Ctrl+Q"));
    addAction(quitAction);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));
}

void MainWindow::toggleFullScreen()
{
    if (isFullScreen())
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    else
        setWindowState(windowState() | Qt::WindowFullScreen);
}

void MainWindow::exitFullScreen()
{
    setWindowState(windowState() & ~Qt::WindowFullScreen);
}

void MainWindow::exitFullScreenOrHide()
{
    if (isFullScreen())
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    else
        QTimer::singleShot(0, this, SLOT(hide()));
}

void MainWindow::toggleHidden()
{
    if (isVisible()) {
        QTimer::singleShot(0, this, [this]() {
            hide();
        });
    } else {
        QTimer::singleShot(0, this, [this]() {
            ensurePolished();
            setWindowState(windowState() & ~Qt::WindowMinimized);
            show();
            raise();
            activateWindow();
        });
    }
}

void MainWindow::quitApplication()
{
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isVisible()) {
        event->ignore();

        QTimer::singleShot(0, this, SLOT(hide()));
    } else {
        quitApplication();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
   QSettings settings("doer", "doer");

   settings.setValue("geometry", QString(saveGeometry().toHex()));

   QMainWindow::resizeEvent(event);
}

void MainWindow::on_textArea_textChanged()
{
    QSettings settings("doer", "doer");

    settings.setValue("text", ui->textArea->toPlainText());
}
