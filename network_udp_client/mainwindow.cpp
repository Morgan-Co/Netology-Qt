#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSockets();

    connect(udpWorker, &UDPworker::sig_dataToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QVariant dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QVariant data, uint32_t data_size, QHostAddress senderAddress)
{
    switch (data.typeId()) {
    case QMetaType::QDateTime: {
        counterPck++;
        if(counterPck % 20 == 0){
            ui->te_result->clear();
        }

        ui->te_result->append("Текущее время: " + data.toString() + ". "
                                                                    "Принято пакетов " + QString::number(counterPck));
        break;
    }
    case QMetaType::QString: {
        counterPck++;
        ui->te_result->append("User: " + data.toString() + ". Accepted packages " + QString::number(counterPck));
        QMessageBox *msg_box = new QMessageBox;
        msg_box->setText("Accept new message from: " + senderAddress.toString() + " Size of message: " + QString::number(data_size));
        msg_box->show();
    }

    default: break;
    }

}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}


void MainWindow::on_pb_send_datagram_clicked() {
    QVariant message = ui->ln_message->text();

    if (message == "") {
        QMessageBox msgBox;
        msgBox.setText("Enter some message");
        msgBox.exec();
        return;
    }

    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    outStr << message;
    udpWorker->SendDatagram(dataToSend);
}




















