#include <QtGui>

#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent)
{
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    addAction(quitAction);
}

TrayMenu::~TrayMenu()
{
    delete quitAction;
}
