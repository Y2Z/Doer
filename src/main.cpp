#include <QApplication>

#include "mainwindow.h"
#include "runguard.h"

QString getUserName()
{
    QString name = qgetenv("USER");

    if (name.isEmpty())
        name = qgetenv("USERNAME");

    return name;
}

int main(int argc, char **argv)
{
    RunGuard guard(getUserName() + "-doer-UwNDcyZjc6MjE1tN2Ma3NGIf5OGx");

    if (!guard.tryToRun())
        return 0;

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
