#pragma once

#include <QMenu>

class TrayMenu : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenu(QWidget *parent = 0);
    ~TrayMenu();

private:
    QAction *toggleAction;
    QAction *quitAction;
};
