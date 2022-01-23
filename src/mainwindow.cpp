#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QShortcut>
#include <QSettings>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString *progName = new QString(PROG_NAME);
    singleInstance = new SingleInstance((QWidget*)this, progName);

    loadSettings();

    setMinimumSize(240, 360);
    setWindowIcon(QIcon(":/images/" PROG_NAME ".svg"));

    applyStyle();
    ui->textArea->setFrameStyle(QFrame::NoFrame);

    bindShortcuts();

    ready = true;
}

MainWindow::~MainWindow(void)
{
    delete ui;
}

void MainWindow::applyStyle(void)
{
    QString styleSheet;

    QFile styleFile(":/stylesheets/" PROG_NAME ".qss");
    styleFile.open(QFile::ReadOnly);
    styleSheet = QLatin1String(styleFile.readAll());
    styleFile.close();

    QFileInfo settingsFileInfo(settings->fileName());
    QFile customStyleFile(settingsFileInfo.absolutePath() + QDir::separator() + PROG_NAME ".qss");
    if (customStyleFile.open(QFile::ReadOnly)) {
        styleSheet += QLatin1String(customStyleFile.readAll());
        customStyleFile.close();
    }

    ui->textArea->setStyleSheet(styleSheet);
}

void MainWindow::loadSettings(void)
{
    settings = new QSettings(QSettings::IniFormat,
                             QSettings::UserScope,
                             PROG_NAME,
                             PROG_NAME,
                             nullptr);

    if (settings->contains("text")) {
        const QString content = settings->value("text").toString();

        ui->textArea->setPlainText(content);
    }

    if (settings->contains("cursor")) {
        const int cursorPosition = settings->value("cursor").toInt();
        QTextCursor newCursor = ui->textArea->textCursor();

        newCursor.setPosition(cursorPosition);

        if (settings->contains("cursor_end")) {
            const int cursorPositionEnd = settings->value("cursor_end").toInt();
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

void MainWindow::bindShortcuts(void)
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
    connect(fullScreenExitAction, SIGNAL(triggered()), this, SLOT(exitFullScreen()));

    // Connect the quit shortcut (Ctrl+Q)
    QAction *quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence("Ctrl+Q"));
    addAction(quitAction);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));
}

void MainWindow::exitFullScreen(void)
{
    setWindowState(windowState() & ~Qt::WindowFullScreen);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    quitApplication();

    QMainWindow::closeEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    windowGeometry = saveGeometry();

    QMainWindow::moveEvent(event);
}

void MainWindow::on_textArea_cursorPositionChanged(void)
{
    if (ready) {
        const QTextCursor currCursor = ui->textArea->textCursor();
        const int currCursorSelStart = currCursor.selectionStart();
        const int currCursorSelEnd = currCursor.selectionEnd();

        settings->setValue("cursor", currCursorSelStart);
        settings->setValue("cursor_end", currCursorSelEnd);
        settings->sync();
    }
}

void MainWindow::on_textArea_textChanged(void)
{
    if (ready) {
        settings->setValue("text", ui->textArea->toPlainText());
        settings->sync();
    }
}

void MainWindow::quitApplication(void)
{
    settings->setValue("geometry", QString(saveGeometry().toHex()));
    settings->sync();

    QApplication::quit();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    windowGeometry = saveGeometry();

    QMainWindow::resizeEvent(event);
}

void MainWindow::toggleFullScreen(void)
{
    if (isFullScreen()) {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() | Qt::WindowFullScreen);
    }
}

bool MainWindow::isAlreadyRunning(void)
{
    const bool raiseExisting = true;
    const bool anotherInstanceRunning = singleInstance->isAlreadyRunning(raiseExisting);

    return anotherInstanceRunning;
}
