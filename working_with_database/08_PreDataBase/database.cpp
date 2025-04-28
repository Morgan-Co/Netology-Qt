#include "database.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();


}

DataBase::~DataBase()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());



    bool status;
    status = dataBase->open();
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request) {
    if (!dataBase->isOpen()) {
        qDebug() << "Database is NOT open!";
        return;
    }
    QTableWidget *tableWg = new QTableWidget();

    if(request == "") {
        QSqlTableModel *model = new QSqlTableModel(nullptr, *dataBase);
        model->setTable("film");
        model->select();

        tableWg->setRowCount(model->rowCount());
        tableWg->setColumnCount(2);
        tableWg->setHorizontalHeaderLabels(QStringList() << "Название фильма" << "Описание фильма");

        for (int row = 0; row < model->rowCount(); ++row) {
            QString title = model->data(model->index(row, model->fieldIndex("title"))).toString();
            QString description = model->data(model->index(row, model->fieldIndex("description"))).toString();

            tableWg->setItem(row, 0, new QTableWidgetItem(title));
            tableWg->setItem(row, 1, new QTableWidgetItem(description));
        }

        delete model;
    } else {
        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query(*dataBase);

        query.prepare(R"(
            SELECT f.title AS "Название фильма", f.description AS "Описание фильма"
            FROM film f
            JOIN film_category fc ON f.film_id = fc.film_id
            JOIN category c ON c.category_id = fc.category_id
            WHERE c.name = :genre)");

        query.bindValue(":genre", request);
        if (!query.exec()) {
            qDebug() << "Query error: " << query.lastError().text();
            delete tableWg;
            return;
        }

        model->setQuery((std::move(query)));
        if (model->lastError().isValid()) {
            qDebug() << "Model error: " << model->lastError().text();
            delete tableWg;
            return;
        }

        tableWg->setRowCount(model->rowCount());
        tableWg->setColumnCount(model->columnCount());
        for (int i = 0; i < model->columnCount(); ++i) {
            tableWg->setHorizontalHeaderItem(i, new QTableWidgetItem(model->headerData(i, Qt::Horizontal).toString()));
        }

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                tableWg->setItem(row, col, item);
            }
        }

        delete model;
    }

    int typeR = 1;
    emit sig_SendDataFromDB(tableWg, typeR);
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
