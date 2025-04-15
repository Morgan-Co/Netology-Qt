#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent) {}

void UDPworker::InitSockets() {
    serviceUdpSocket = new QUdpSocket(this);
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

void UDPworker::ReadDatagram(QNetworkDatagram datagram) {
    QByteArray byteData;
    byteData = datagram.data();

    QDataStream inStr(&byteData, QIODevice::ReadOnly);
    QVariant data;
    inStr >> data;

    emit sig_dataToGUI(data, byteData.size(), datagram.senderAddress());
}

void UDPworker::SendDatagram(QByteArray data) {
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}


void UDPworker::readPendingDatagrams() {
    while(serviceUdpSocket->hasPendingDatagrams()){
        QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
        ReadDatagram(datagram);
    }
}









