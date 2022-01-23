#pragma once

#include <QAction>
#include <QMainWindow>
#include <QSettings>

#include "singleinstance.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    bool isAlreadyRunning(void);

public slots:
    void exitFullScreen(void);
    void quitApplication(void);
    void toggleFullScreen(void);

protected:
    void closeEvent(QCloseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    bool ready = false;

private slots:
    void on_textArea_cursorPositionChanged(void);
    void on_textArea_textChanged(void);

private:
    QSettings *settings;
    Ui::MainWindow *ui;
    QByteArray windowGeometry;
    SingleInstance *singleInstance;

    void applyStyle(void);
    void bindShortcuts(void);
    void loadSettings(void);
};
