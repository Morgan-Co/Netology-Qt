#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent) {}

void UDPworker::InitSockets() {
    datetimeUdpSocket = new QUdpSocket(this);
    datetimeUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    messageUdpSocket = new QUdpSocket(this);
    messageUdpSocket->bind(QHostAddress::LocalHost, MESSAGE_PORT);

    connect(datetimeUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(messageUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

void UDPworker::ReadDatagram(QNetworkDatagram datagram) {
    QByteArray byteData;
    byteData = datagram.data();

    QDataStream inStr(&byteData, QIODevice::ReadOnly);
    QVariant data;
    inStr >> data;

    emit sig_dataToGUI(data);
}

void UDPworker::SendDatagram(QByteArray data, ESocketType socketType) {
    switch (socketType) {
    case ESocketType::DateTimeSocket: {
        datetimeUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
        break;
    }
    case ESocketType::MessageSocket: {
        messageUdpSocket->writeDatagram(data, QHostAddress::LocalHost, MESSAGE_PORT);
    }
    default: break;
    }
}


void UDPworker::readPendingDatagrams() {
    QUdpSocket *senderSocket = qobject_cast<QUdpSocket*>(sender());
    if (!senderSocket) return;

    while(senderSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = senderSocket->receiveDatagram();
        ReadDatagram(datagram);
    }
}









