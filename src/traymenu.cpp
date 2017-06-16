#include <QtGui>

#include "traymenu.h"


TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent)
{
    toggleAction = new QAction(tr("Toggle"), this);
    quitAction = new QAction(tr("Quit"), this);

    connect(toggleAction, SIGNAL(triggered()), parent, SLOT(toggleHidden()));
    connect(quitAction, SIGNAL(triggered()), parent, SLOT(quitApplication()));

    addAction(toggleAction);
    addAction(quitAction);
}

TrayMenu::~TrayMenu()
{
    delete toggleAction;
    delete quitAction;
}
