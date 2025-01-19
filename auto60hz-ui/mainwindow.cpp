#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../main.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Auto 60hz");
    connect(ui->apply, &QPushButton::clicked, this, &MainWindow::onApplyClicked);
    connect(ui->startup, &QCheckBox::toggled, this, &MainWindow::startupCheckboxChanged);
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onApplyClicked()
{
    // Disable the Apply button to prevent rapid clicking
    ui->apply->setEnabled(false);
    // Retrieve values from input fields
    QString highText = ui->high->text();
    QString lowText = ui->low->text();

    // Convert to integer values
    int high = highText.toInt();
    int low = lowText.toInt();

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
    // Create a QSettings object to read the settings from the file
    QSettings settings("settings.ini", QSettings::IniFormat);

    // Read the saved values from the INI file
    int high = settings.value("High", 0).toInt();  // Default value 0 if not found
    int low = settings.value("Low", 0).toInt();    // Default value 0 if not found

    // Set the values in the input fields
    ui->high->setText(QString::number(high));
    ui->low->setText(QString::number(low));
    SetHighLowValues(high, low);
    startThread();
}

// Function to end the thread
void MainWindow::endThread() {
    shouldStop.store(true);  // Signal thread to stop
    if (logicThread.joinable()) {
        logicThread.join();  // Wait for the thread to finish
    }
}
// fuction to start the thread
void MainWindow::startThread()
{
    // Reset the stop flag to allow the new thread to run
    shouldStop.store(false);
    // Start a new thread for mainScript
    logicThread = std::thread(mainScript);  // Start the new thread
    logicThread.detach();  // Detach it so it runs independently
}

void MainWindow::startupCheckboxChanged(bool checked)
{
    // Handle the checkbox state change immediately
    handleStartup(checked);

    // Save the checkbox state for future runs
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("Startup", checked);
}
// function to handle startup
void MainWindow::handleStartup(bool checked)
{
    // Get the path to the application executable
    QString appPath = QCoreApplication::applicationFilePath();

    // Registry key to add/remove the application from startup
    QSettings registry("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (checked) {
        // Add application to registry for startup
        registry.setValue("Auto60hz", appPath);
    } else {
        // Remove application from registry
        registry.remove("Auto60hz");
    }
}
