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

void Database::ConnectToDataBase() {
    QVector<QString> data;
    data.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    data[hostName] = "localhost";
    data[dbName] = DB_NAME;
    data[login] = "postgres";
    data[pass] = "Zerg0987";
    data[port] = "5432";

    data_base->setHostName(data[hostName]);
    data_base->setDatabaseName(data[dbName]);
    data_base->setUserName(data[login]);
    data_base->setPassword(data[pass]);
    data_base->setPort(data[port].toInt());

    status_connection = data_base->open();
    emit sig_SendStatusConnection(status_connection);

    QString request = "SELECT airport_name->>'ru' AS name, airport_code FROM bookings.airports_data ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*data_base);
    QSqlError error;

    if (status_connection) {
        if(!query->exec(request)){
            error = query->lastError();

            emit
        } else {
            model_main->setQuery(*query);

            emit sig_SendQueryFromDB(model_main);
            emit sig_SendListAirports(model_main);
        }
    }
    delete query;
}

void Database::DisconnectFromDataBase(QString nameDb) {
    *data_base = QSqlDatabase::database(nameDb);
    data_base->close();
}

QSqlError Database::GetLastError() {
    return data_base->lastError();
}


//!< Requests
void Database::GetDataArrivals(const QString& airport_code, const QString& date) {
    QString converted_date = ConvertDate(date);
    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                      "WHERE (f.arrival_airport  = '" + airport_code + "' AND f.scheduled_arrival::date = date('" + converted_date + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*data_base);
    QSqlError error;

    if(!query->exec(request)){
        error = query->lastError();
    }

    model_main->setQuery(*query);
    emit sig_SendDataToArrivals(model_main);
    delete query;
}

void Database::GetDataDepartures(const QString &airport_code, const QString& date) {
    QString converted_date = ConvertDate(date);
    QString request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                      "WHERE (f.departure_airport  = '" + airport_code + "' AND f.scheduled_departure::date = date('" + converted_date + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*data_base);
    QSqlError error;

    if(!query->exec(request)){
        error = query->lastError();
    }

    model_main->setQuery(*query);
    emit sig_SendDataToDepartures(model_main);
    delete query;
}

void Database::GetDataPerYear(const QString &airport_code) {
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) AS Month "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airport_code + "' or arrival_airport = '" + airport_code + "') "
                      "GROUP BY Month";

    QSqlQuery* query = new QSqlQuery(*data_base);
    QSqlError error;

    if(!query->exec(request)){
        error = query->lastError();
    }

    model_statistic->setQuery(*query);
    emit sig_SendDataPerYear(model_statistic);
    delete query;
}

void Database::GetDataPerMonth(const QString &airport_code) {
    QString request = "SELECT count(flight_no), date_part('month', date_trunc('day', scheduled_departure)), date_trunc('day', scheduled_departure) AS Day "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airport_code + "' or arrival_airport = '" + airport_code + "') "
                      "GROUP BY Day";

    QSqlQuery* query = new QSqlQuery(*data_base);
    QSqlError error;

    if(!query->exec(request)){
        error = query->lastError();
    }

    model_statistic->setQuery(*query);
    emit sig_SendDataPerMonth(model_statistic);
    delete query;
}


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
