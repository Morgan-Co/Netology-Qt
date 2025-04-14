#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TCPclient(this);
    //Доступность полей по умолчанию
    ui->le_data->setEnabled(false);
    ui->pb_request->setEnabled(false);
    ui->lb_connectStatus->setText("Отключено");
    ui->lb_connectStatus->setStyleSheet("color: red");


    //При отключении меняем надписи и доступность полей.
    connect(client, &TCPclient::sig_Disconnected, this, [&]{

        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color: red");
        ui->pb_connect->setText("Подключиться");
        ui->le_data->setEnabled(false);
        ui->pb_request->setEnabled(false);
        ui->spB_port->setEnabled(true);
        ui->spB_ip1->setEnabled(true);
        ui->spB_ip2->setEnabled(true);
        ui->spB_ip3->setEnabled(true);
        ui->spB_ip4->setEnabled(true);

    });

    connect(client, &TCPclient::sig_Success, this, &MainWindow::DisplayConnectStatus);
    connect(client, &TCPclient::sig_sendTime, this, &MainWindow::DisplayTime);
    connect(client, &TCPclient::sig_sendFreeSize, this, &MainWindow::DisplayFreeSpace);
    connect(client, &TCPclient::sig_SendReplyForSetData, this, &MainWindow::SetDataReply);
    connect(client, &TCPclient::sig_sendStat, this, &MainWindow::DisplayStat);


 /*
  * Соединяем сигналы со слотами
 */


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Группа методо отображения различных данных
 */
void MainWindow::DisplayTime(QDateTime time) {
    QString result_str = "Current time and date " + time.toString();
    ui->tb_result->append(result_str);
}
void MainWindow::DisplayFreeSpace(uint32_t freeSpace) {
    QString result_str = "Free space: " + QString::number(freeSpace);
    ui->tb_result->append(result_str);
}
void MainWindow::SetDataReply(QString replyString) {
    ui->tb_result->append("User: " + replyString);
}
void MainWindow::DisplayStat(StatServer stat) {
    ui->tb_result->append("Clients: " + QString::number(stat.clients));
    ui->tb_result->append("Incoming bytes: " + QString::number(stat.incBytes));
    ui->tb_result->append("Sent bytes: " + QString::number(stat.sendBytes));
    ui->tb_result->append("Rev packages: " + QString::number(stat.revPck));
    ui->tb_result->append("Work time: " + QString::number(stat.workTime));
}
void MainWindow::DisplayError(uint16_t error) {
    switch (error) {
    case ERR_NO_FREE_SPACE:
    case ERR_NO_FUNCT:
    default:
        break;
    }
}
/*!
 * \brief Метод отображает квитанцию об успешно выполненном сообщениии
 * \param typeMess ИД успешно выполненного сообщения
 */
void MainWindow::DisplaySuccess(uint16_t typeMess) {
    switch (typeMess) {
    case CLEAR_DATA:
    default:
        break;
    }
}

/*!
 * \brief Метод отображает статус подключения
 */
void MainWindow::DisplayConnectStatus(uint16_t status) {
    if(status == ERR_CONNECT_TO_HOST){

        ui->tb_result->append("Ошибка подключения к порту: " + QString::number(ui->spB_port->value()));

    }
    else{
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color: green");
        ui->pb_connect->setText("Отключиться");
        ui->spB_port->setEnabled(false);
        ui->pb_request->setEnabled(true);
        ui->spB_ip1->setEnabled(false);
        ui->spB_ip2->setEnabled(false);
        ui->spB_ip3->setEnabled(false);
        ui->spB_ip4->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки подключения/отключения
 */
void MainWindow::on_pb_connect_clicked()
{
    if(ui->pb_connect->text() == "Подключиться"){

        uint16_t port = ui->spB_port->value();

        QString ip = ui->spB_ip4->text() + "." +
                     ui->spB_ip3->text() + "." +
                     ui->spB_ip2->text() + "." +
                     ui->spB_ip1->text();

        client->ConnectToHost(QHostAddress(ip), port);

    }
    else{

        client->DisconnectFromHost();
    }
}

/*
 * Для отправки сообщения согласно ПИВ необходимо
 * заполнить заголовок и передать его на сервер. В ответ
 * сервер вернет информацию в соответствии с типом сообщения
*/
void MainWindow::on_pb_request_clicked() {

   ServiceHeader header;

   header.id = ID;
   header.status = STATUS_SUCCES;
   header.len = 0;

   switch (ui->cb_request->currentIndex()){
       //Получить время
        case EDataTypeID::GetTime: {
            header.idData = GET_TIME;
            break;
        }
       //Получить свободное место
        case EDataTypeID::GetFreeSpace: {
            header.idData = GET_SIZE;
            break;
        }
       //Получить статистику
        case EDataTypeID::GetStats: {
            header.idData = GET_STAT;
            break;
        }
       //Отправить данные
        case EDataTypeID::SendData: {
            header.idData = SET_DATA;
            QString str_data = ui->le_data->text();
            header.len = str_data.size();
            client->SendData(header, str_data);
            return;
        }
       //Очистить память на сервере
        case EDataTypeID::FreeData: {
            header.idData = CLEAR_DATA;
            break;
        }
       default:
       ui->tb_result->append("Такой запрос не реализован в текущей версии");
       return;
   }
   client->SendRequest(header);
}

/*!
 * \brief Обработчик изменения индекса запроса
 */
void MainWindow::on_cb_request_currentIndexChanged(int index) {
    //Разблокируем поле отправления данных только когда выбрано "Отправить данные"
    if(ui->cb_request->currentIndex() == 3){
        ui->le_data->setEnabled(true);
    }
    else{
        ui->le_data->setEnabled(false);
    }
}

