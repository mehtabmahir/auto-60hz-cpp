#include "mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set the current working directory to the directory of the executable
    QString appDir = QCoreApplication::applicationDirPath();
    QDir::setCurrent(appDir);
    a.setWindowIcon(QIcon(":/resources/icon.png"));
    MainWindow w;
    QStringList arguments = QCoreApplication::arguments();
    // hide on startup
    if (QCoreApplication::arguments().contains("--startup")) {
        w.hide();
        w.trayIcon->showMessage("Auto 60hz", "Started with Windows.");
    }
    else
        w.show();
    return a.exec();
}
