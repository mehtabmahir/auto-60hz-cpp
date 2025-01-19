#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onApplyClicked();
    void saveSettings(int high, int low);
    void loadSettings();
    void endThread();
    void startThread();
    void startupCheckboxChanged(bool checked);
    void handleStartup(bool checked);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
