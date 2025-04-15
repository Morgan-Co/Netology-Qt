#include "database.h"

#include <QSqlQuery>
#include <QSqlQueryModel>

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
    QString str_query = request != "" ? R"(
        SELECT title, description
        FROM film f
        JOIN film_category fc ON f.film_id = fc.film_id
        JOIN category c ON c.category_id = fc.category_id
        WHERE c.name = :genre
    )" : R"(
        SELECT title, description
        FROM film f
        JOIN film_category fc ON f.film_id = fc.film_id
        JOIN category c ON c.category_id = fc.category_id
    )";

    if (dataBase->isOpen()) {
        QSqlQuery query(*dataBase);

        query.prepare(str_query);
        if (request != "") query.bindValue(":genre", request);
        if (!query.exec()) {
            qDebug() << "Query execution failed:" << query.lastError().text();
            return;
        }

        qDebug() << "Number of rows returned:" << query.size();

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(std::move(query));

        if (model->lastError().isValid()) {
            qDebug() << "Model error:" << model->lastError().text();
            return;
        }

        qDebug() << "Database is open";
        qDebug() << "Executed request:" << str_query;
        qDebug() << "Bound value for :genre:" << request;

        QTableWidget *tableWg = new QTableWidget();
        tableWg->setRowCount(model->rowCount());
        tableWg->setColumnCount(model->columnCount());

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QTableWidgetItem *newItem = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                tableWg->setItem(row, col, newItem);
            }
        }

        int typeR = 1;
        emit sig_SendDataFromDB(tableWg, typeR);
    } else {
        qDebug() << "Database is NOT open!";
    }
}




/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
