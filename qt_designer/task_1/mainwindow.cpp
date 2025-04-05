#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->push_dialog, &QPushButton::clicked, this, [=]() {
        Dialog dialog(this);
        dialog.exec();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

