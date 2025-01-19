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
    bool highOk, lowOk;
    int high = highText.toInt(&highOk);
    int low = lowText.toInt(&lowOk);

    if (high < 1 || low <= 1) {
        return; // invalid input
    }

    // Pass values to main program
    //SetHighLowValues(high, low);

    // Save the values to a file
    //SaveValuesToFile(high, low);
}



