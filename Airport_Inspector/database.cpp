#include "database.h"

DataBase::DataBase(QObject *parent) : QObject{parent}
{
    dataBase_ = new QSqlDatabase();
    modelQueryMain_= new QSqlQueryModel(this);
    modelQueryStatistics_ = new QSqlQueryModel(this);
    statusConnection_ = false;
}

DataBase::~DataBase()
{
    delete dataBase_;
}

void DataBase::addDataBase(QString driver, QString nameDB)
{
    *dataBase_ = QSqlDatabase::addDatabase(driver, nameDB);
}

bool DataBase::getStatusConnection()
{
    return statusConnection_;
}

void DataBase::connectToDB()
{
    //Данные для подключения к БД.
    QVector<QString> data;
    data.resize(port + 1);

    data[hostName] = "981757-ca08998.tmweb.ru";
    data[port] = "5432";
    data[dbName] = "demo";
    data[login] = "netology_usr_cpp";
    data[pass] = "CppNeto3";

    dataBase_->setHostName(data[hostName]);
    dataBase_->setPort(data[port].toInt());
    dataBase_->setDatabaseName(data[dbName]);
    dataBase_->setUserName(data[login]);
    dataBase_->setPassword(data[pass]);

    statusConnection_ = dataBase_->open();

    QString request = "SELECT airport_name->>'ru' AS name, airport_code FROM bookings.airports_data ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain_->setQuery(*query);

    emit sig_SendQueryFromDB(modelQueryMain_);
    emit sig_SendStatusConnection(statusConnection_);
    emit sig_SendListAirports(modelQueryMain_);
    delete query;
}

void DataBase::disconnectFromDB(QString nameDb)
{
    *dataBase_ = QSqlDatabase::database(nameDb);
    dataBase_->close();
}

QSqlError DataBase::getLastError()
{
    return dataBase_->lastError();
}

void DataBase::getDataArrivals(const QString& airportCode, const QString& date)
{
    QString parsedDate = convertInputDate(date);
    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                      "WHERE (f.arrival_airport  = '" + airportCode + "' AND f.scheduled_arrival::date = date('" + parsedDate + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain_->setQuery(*query);
    emit sig_SendDataToArrivals(modelQueryMain_);
    delete query;
}

void DataBase::getDataDepartures(const QString &airportCode, const QString& date)
{
    QString parsedDate = convertInputDate(date);
    QString request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                      "WHERE (f.departure_airport  = '" + airportCode + "' AND f.scheduled_departure::date = date('" + parsedDate + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain_->setQuery(*query);
    emit sig_SendDataToDepartures(modelQueryMain_);
    delete query;
}

void DataBase::getDataPerYear(const QString &airportCode)
{
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) AS Month "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Month";

    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryStatistics_->setQuery(*query);
    emit sig_SendDataPerYear(modelQueryStatistics_);
    delete query;
}

void DataBase::getDataPerMonth(const QString &airportCode)
{
    QString request = "SELECT count(flight_no), date_part('month', date_trunc('day', scheduled_departure)), date_trunc('day', scheduled_departure) AS Day "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Day";

    QSqlQuery* query = new QSqlQuery(*dataBase_);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryStatistics_->setQuery(*query);
    emit sig_SendDataPerMonth(modelQueryStatistics_);
    delete query;
}

QString DataBase::convertInputDate(const QString &date)
{
    QString day, month, year;
    for(int i = 0; i < date.size(); ++i)
    {
        if(i < 2)
        {
            day+=date[i];
        }
        if(i > 2 && i < 5)
        {
            month+=date[i];
        }
        if(i > 5)
        {
            year+=date[i];
        }
    }
    QString resultStr = year + "-" + month + "-" + day;
    return resultStr;
}
