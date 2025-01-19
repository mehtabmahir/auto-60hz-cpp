#include "mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set the current working directory to the directory of the executable
    QString appDir = QCoreApplication::applicationDirPath();
    QDir::setCurrent(appDir);
    MainWindow w;
    w.show();
    return a.exec();
}
