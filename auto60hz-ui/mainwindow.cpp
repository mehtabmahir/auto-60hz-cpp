#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../main.cpp"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Auto 60hz");
    connect(ui->apply, &QPushButton::clicked, this, &MainWindow::onApplyClicked);
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onApplyClicked()
{
    // Retrieve values from input fields
    QString highText = ui->high->text();
    QString lowText = ui->low->text();

    // Convert to integer values
    int high = highText.toInt();
    int low = lowText.toInt();

    if (high < 1 || low <= 1) {
        return; // invalid input
    }

    // Pass values to main program
    SetHighLowValues(high, low);

    // Save the values to a file
    saveSettings(high, low);
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
}

