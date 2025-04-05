#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setCheckable(true);

    ui->progressBar->setRange(0, 10);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    if (ui->progressBar->value() >= 10) ui->progressBar->setValue(0);
    else ui->progressBar->setValue(ui->progressBar->value() + 1);
}
