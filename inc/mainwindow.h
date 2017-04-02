#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>

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
    void toggleFullScreen();
    void exitFullScreen();
    void exitFullScreenOrHide();
    void quit();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent* event) override;
    void toggleHidden();

private slots:
    void on_textArea_textChanged();

private:
    Ui::MainWindow *ui;
    void initialize();
    void bindShortcuts();

    void setIcon();
    void setStyle();

    void loadSettings();

    QSystemTrayIcon trayIcon;
};
