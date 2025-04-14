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
    void SendDatagram(QByteArray data, ESocketType socketType);


private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* datetimeUdpSocket;
    QUdpSocket* messageUdpSocket;

signals:
    void sig_dataToGUI(QVariant data);

};

#endif // UDPWORKER_H
