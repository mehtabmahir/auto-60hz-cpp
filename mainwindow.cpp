#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./auto60hz.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Auto 60hz");
    setFixedSize(240, 140);
    // Tray Icon Setup
    setupTrayIcon();

    connect(ui->apply, &QPushButton::clicked, this, &MainWindow::onApplyClicked);
    connect(ui->startup, &QCheckBox::toggled, this, &MainWindow::startupCheckboxChanged);
    connect(ui->donate, &QPushButton::clicked, this, &MainWindow::onDonateClicked);
    connect(ui->github, &QPushButton::clicked, this, &MainWindow::onGithubClicked);
    connect(ui->pause, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPauseClicked()
{
    if (ui->pause->text() == "Pause") {
        trayIcon->showMessage("Auto 60hz", "Pausing applicating.");
        ui->pause->setText("Resume");
        endThread();
    }
    else {
        trayIcon->showMessage("Auto 60hz", "Resuming application.");
        ui->pause->setText("Pause");
        startThread();
    }
}

void MainWindow::onApplyClicked()
{
    // Disable the Apply button to prevent rapid clicking
    ui->apply->setEnabled(false);
    // Retrieve values from input fields
    QString highText = ui->high->text();
    QString lowText = ui->low->text();

    // Convert to integer values
    high = highText.toInt();
    low = lowText.toInt();

    if (high < 1 || low <= 1) {
        ui->apply->setEnabled(true);  // Re-enable the Apply button
        return; // invalid input
    }

    // Pass values to main program
    SetHighLowValues(high, low);

    // Save the values to a file
    saveSettings(high, low);

    endThread();
    startThread();

    // enable apply button
    ui->apply->setEnabled(true);
}

void MainWindow::onDonateClicked()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/donate/?business=5FM6Y27A3CK58&no_recurring=0&currency_code=USD"));
}

void MainWindow::onGithubClicked()
{
    QDesktopServices::openUrl(QUrl("https://www.github.com/mehtabmahir"));
}

void MainWindow::saveSettings(int high, int low)
{
    // Create a QSettings object to write the settings to a file
    QSettings settings("settings.ini", QSettings::IniFormat);

    // Write the values to the INI file
    settings.setValue("High", high);
    settings.setValue("Low", low);
}

void MainWindow::loadSettings()
{
    // Get the directory where the executable is located
    QString appDir = QCoreApplication::applicationDirPath();

    // Construct the full path to settings.ini
    QString settingsFilePath = QString("%1/settings.ini").arg(appDir);

    // Load settings using the full path
    QSettings settings(settingsFilePath, QSettings::IniFormat);

    // Read the saved values from the INI file
    int high = settings.value("High", 0).toInt();  // Default value 0 if not found
    int low = settings.value("Low", 0).toInt();    // Default value 0 if not found

    // Load the startup checkbox state
    bool startupChecked = settings.value("Startup", false).toBool();  // Default to false if not found
    ui->startup->setChecked(startupChecked);  // Set the checkbox state
    handleStartup(startupChecked);

    // Set the values in the input fields
    ui->high->setText(QString::number(high));
    ui->low->setText(QString::number(low));
    SetHighLowValues(high, low);
    startThread();
}

// Function to end the thread
void MainWindow::endThread()
{
    // Signal mainScript() loop to exit
    shouldStop.store(true);

    // If there's an active QThread, wait for it to finish
    if (logicThread) {
        logicThread->quit();   // Ask the thread's event loop to quit
        logicThread->wait();   // Wait until mainScript() returns
        delete logicThread;    // Clean up
        logicThread = nullptr;
    }
}


// fuction to start the thread
void MainWindow::startThread()
{
    // First, end any existing thread so we don't spawn duplicates
    endThread();

    // Reset stop flag so mainScript() won't exit immediately
    shouldStop.store(false);

    // Create a new QThread that runs mainScript in its own function
    logicThread = QThread::create([]() {
        mainScript(); // your existing function from main.cpp
    });

    // Start the QThread
    logicThread->start();
}


void MainWindow::startupCheckboxChanged(bool checked)
{
    // Handle the checkbox state change immediately
    handleStartup(checked);

    // Save the checkbox state for future runs
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("Startup", checked);
}
void MainWindow::handleStartup(bool checked)
{
    // Access the registry for startup applications
    QSettings bootUpSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    // Get the application's executable path
    QString appPath = QCoreApplication::applicationFilePath();

    // Replace any forward slashes with backslashes
    appPath.replace("/", "\\");

    // Surround the path with quotes
    QString quotedAppPath = QString("\"%1\" --startup").arg(appPath);

    // If the checkbox is checked, add the application to startup
    if (checked) {
        bootUpSettings.setValue("Auto 60hz", quotedAppPath);
    } else {
        // Otherwise, remove the application from startup
        bootUpSettings.remove("Auto 60hz");
    }
}

void MainWindow::setupTrayIcon()
{
    // Create the tray icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resources/icon.png")); // Set your desired icon
    trayIcon->setToolTip("Auto 60hz");

    // Create the tray menu
    trayMenu = new QMenu(this);
    QAction *pauseAction = trayMenu->addAction("Pause");
    QAction *restoreAction = trayMenu->addAction("Restore");
    QAction *quitAction = trayMenu->addAction("Quit");

    // Connect menu actions
    connect(pauseAction, &QAction::triggered, this, [pauseAction, this]() {
        if (pauseAction->text() == "Pause")
            pauseAction->setText("Resume");
        else
            pauseAction->setText("Pause");
        onPauseClicked();
    });

    connect(restoreAction, &QAction::triggered, this, [this]() {
        this->showNormal();  // Restore the window
        this->raise();       // Bring it to the front
        this->activateWindow(); // Focus the window
    });

    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    // Handle tray icon activation
    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Trigger: // Single click
        case QSystemTrayIcon::DoubleClick: // Double click
            this->showNormal();  // Restore the window
            this->raise();       // Bring it to the front
            this->activateWindow(); // Focus the window
            break;
        case QSystemTrayIcon::MiddleClick: // Middle click, if supported
            trayIcon->showMessage("Auto 60hz", "Application is running!", QSystemTrayIcon::Information, 3000);
            break;
        default:
            break;
        }
    });
}
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            hide();  // Hide the window when minimized
            trayIcon->showMessage("Auto 60hz", "The application is minimized to the system tray.");
        }
    }
    QMainWindow::changeEvent(event);  // Pass the event to the base class
}
