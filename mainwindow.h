#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <QTimer>

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

private slots:
    void onPauseClicked();
    void onApplyClicked();
    void onDonateClicked();
    void onGithubClicked();
    void saveSettings(int high, int low);
    void loadSettings();
    void endThread();
    void startThread();
    void startupCheckboxChanged(bool checked);
    void handleStartup(bool checked);

private:
    Ui::MainWindow *ui;
    QMenu *trayMenu;
    QThread *logicThread = nullptr;
    int high, low;
    QTimer *refreshPollTimer = nullptr;
};
#endif // MAINWINDOW_H
