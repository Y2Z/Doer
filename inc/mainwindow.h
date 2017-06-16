#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QSettings>

#include "traymenu.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toggleHidden();
    void toggleFullScreen();
    void exitFullScreen();
    void exitFullScreenOrHide();
    void quitApplication();

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event) override;

    bool ready = false;

private slots:
    void on_textArea_textChanged();
    void on_textArea_cursorPositionChanged();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QSystemTrayIcon *trayIcon;
    TrayMenu *trayMenu;

    void bindShortcuts();

    void setIcon();
    void setStyle();

    void loadSettings();
};
