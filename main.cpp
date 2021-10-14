#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.showMaximized();

    QStringList args = a.arguments();
    if (args.count() == 2)
    {
        QString filename = args[1];
        w.openFile(filename);
    }

    return a.exec();
}
