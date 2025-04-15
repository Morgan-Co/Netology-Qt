#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define MESSAGE_PORT 8080

enum ESocketType {
    MessageSocket,
    DateTimeSocket
};

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSockets( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data);


private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;

signals:
    void sig_dataToGUI(QVariant data, uint32_t data_size, QHostAddress senderAddress);

};

#endif // UDPWORKER_H
