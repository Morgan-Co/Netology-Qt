#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "stopwatch.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->push_lap->setDisabled(true);

    connect(&stopwatch, &Stopwatch::timeUpdated, this, &MainWindow::updateDisplay);

    connect(ui->push_start_stop, &QPushButton::clicked, this, &MainWindow::toggleStartStop);
    connect(ui->push_reset, &QPushButton::clicked, this, [=]() {
        stopwatch.reset();
        ui->timer_value->setText("00.00");
        ui->lap_list->clear();
    });
    connect(ui->push_lap, &QPushButton::clicked, this, [=]() {
        stopwatch.lap();
    });
    connect(&stopwatch, &Stopwatch::lapsUpdated, this, &MainWindow::updateLaps);
}

void MainWindow::toggleStartStop() {
    if (!stopwatch.isRunning()) {
        stopwatch.start();
        ui->push_start_stop->setText("Stop");
        ui->push_lap->setDisabled(false);

    } else {
        stopwatch.stop();
        ui->push_start_stop->setText("Start");
        ui->push_lap->setDisabled(true);
    }
}

void MainWindow::updateDisplay(const QString &time) {
    ui->timer_value->setText(time);
}

void MainWindow::updateLaps(const QList<Lap> &laps) {
    ui->lap_list->clear();
    for (int i = 0; i < laps.size(); ++i) {
        int currentLap = laps[i].lapTime;
        int seconds = currentLap / 1000;
        int ms = (currentLap % 1000) / 60;

        QString formatedLap = QString("Lap %1: %2.%3s")
                                   .arg(laps[i].lapsCount)
                                   .arg(seconds, 2, 10, QChar('0'))
                                  .arg(ms, 2, 10, QChar('0'));


        ui->lap_list->addItem(formatedLap);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

