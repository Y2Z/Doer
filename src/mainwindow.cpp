#include <QSettings>
#include <QCloseEvent>
#include <QShortcut>
#include <QTimer>
#include <QFileInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();

    setMinimumSize(240, 360);

    setIcon();

    setStyle();
    ui->textArea->setFrameStyle(QFrame::NoFrame);

    bindShortcuts();

    trayMenu = new TrayMenu(0);
    trayIcon = new QSystemTrayIcon(0);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/images/tray.png"));
    trayIcon->show();

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this,
                     [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            toggleHidden();
        }
    });

    ready = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                             "doer", "doer", nullptr);

    if (settings->contains("text")) {
        QString content = settings->value("text").toString();

        ui->textArea->setPlainText(content);
    }

    if (settings->contains("cursor")) {
        int cursorPosition = settings->value("cursor").toInt();
        QTextCursor newCursor = ui->textArea->textCursor();

        newCursor.setPosition(cursorPosition);

        if (settings->contains("cursor_end")) {
            int cursorPositionEnd = settings->value("cursor_end").toInt();
            newCursor.setPosition(cursorPositionEnd, QTextCursor::KeepAnchor);
        }

        ui->textArea->setTextCursor(newCursor);
        ui->textArea->ensureCursorVisible() ;
    }

    if (settings->contains("geometry")) {
        restoreGeometry(
            QByteArray::fromHex(settings->value("geometry").toByteArray())
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
    QString styleSheet;

    QFile styleFile(":/styles/default.qss");
    styleFile.open(QFile::ReadOnly);
    styleSheet = QLatin1String(styleFile.readAll());
    styleFile.close();

    QFileInfo settingsFileInfo(settings->fileName());
    QFile customStyleFile(settingsFileInfo.absolutePath() + "/doer.qss");
    if (customStyleFile.open(QFile::ReadOnly)) {
        puts("путц");
        styleSheet += QLatin1String(customStyleFile.readAll());
        customStyleFile.close();
    }

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
    if (ready) {
        settings->setValue("geometry", QString(saveGeometry().toHex()));
    }

    QMainWindow::resizeEvent(event);
}

void MainWindow::on_textArea_textChanged()
{
    if (ready) {
        settings->setValue("text", ui->textArea->toPlainText());
    }
}

void MainWindow::on_textArea_cursorPositionChanged()
{
    if (ready) {
        QTextCursor currCursor = ui->textArea->textCursor();
        int currCursorSelStart = currCursor.selectionStart();
        int currCursorSelEnd = currCursor.selectionEnd();

        settings->setValue("cursor", currCursorSelStart);
        settings->setValue("cursor_end", currCursorSelEnd);
    }
}
