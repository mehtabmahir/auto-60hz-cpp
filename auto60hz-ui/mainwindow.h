#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeEvent(QEvent *event);
    void setupTrayIcon();
    QSystemTrayIcon *trayIcon;

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);

private slots:
    void onApplyClicked();
    void saveSettings(int high, int low);
    void loadSettings();
    void endThread();
    void startThread();
    void startupCheckboxChanged(bool checked);
    void handleStartup(bool checked);
    void restartApplication();

private:
    Ui::MainWindow *ui;
    QMenu *trayMenu;
};
#endif // MAINWINDOW_H
