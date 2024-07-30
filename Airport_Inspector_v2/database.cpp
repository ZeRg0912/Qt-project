#include "database.h"

Database::Database(QObject *parent) : QObject{parent} {
    data_base = new QSqlDatabase();
}

Database::~Database() {
    delete data_base;
}


//!< Main methods
void Database::AddDataBase(QString driver, QString nameDB) {
    *data_base = QSqlDatabase::addDatabase(driver, nameDB);
}

void Database::ConnectToDB() {
    data_base->setHostName("localhost");
    data_base->setDatabaseName(DB_NAME);
    data_base->setUserName("postgres");
    data_base->setPassword("Zerg0987");
    data_base->setPort(5432);

    status_connection = data_base->open();
    emit sig_SendStatusConnection(status_connection);
}

void Database::DisconnectFromDataBase(QString nameDb) {
    *data_base = QSqlDatabase::database(nameDb);
    data_base->close();
}

QSqlError Database::GetLastError() {
    return data_base->lastError();
}

void Database::RequestToDB(const QString& request, QTableView* table_view) {
    if (status_connection) {
        QSqlError err;
        query_model = new QSqlQueryModel(this);
        query_model->setQuery(request, *data_base);
        query_model->setHeaderData(0, Qt::Horizontal, tr("Аэропорт"));
        query_model->setHeaderData(1, Qt::Horizontal, tr("Вылет"));
        query_model->setHeaderData(2, Qt::Horizontal, tr("Прилет"));

        err = query_model->lastError();

        if (err.type() != QSqlError::NoError) {
            emit sig_SendStatusRequest(err);
        } else {
            table_view->setModel(query_model);
        }
    }
}

//!< Requests


//!< UTILS
QString Database::ConvertDate(const QString &date) {
    QString day, month, year;

    for(int i = 0; i < date.size(); i++)
    {
        if(i < 2) {
            day += date[i];
        }

        if((i > 2) && (i < 5)) {
            month += date[i];
        }

        if(i > 5) {
            year += date[i];
        }
    }

    QString result = year + "-" + month + "-" + day;
    return result;
}
