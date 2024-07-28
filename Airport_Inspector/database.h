#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"


class DataBase : public QObject
{
    Q_OBJECT
public:

    /*!
     *  @brief Перечисление полей данных.
     */
    enum fieldsForConnect{
        hostName = 0, //<! Имя хоста.
        dbName = 1, //<! Имя базы данных.
        login = 2, //<! Логин.
        pass = 3, //<! Пароль.
        port = 4 //<! Порт.
    };

    DataBase(QObject *parent = nullptr);
    ~DataBase();

    /*!
     *  @brief Добавить БД.
     *  @param driver Драйвер БД.
     *  @param nameDB Имя БД.
     */
    void addDataBase(QString driver, QString nameDB = "");

    /*!
     *  @brief Получить статус подключения.
     */
    bool getStatusConnection();

    /*!
     *  @brief Подключиться к БД.
     */
    void connectToDB();

    /*!
     *  @brief Отключиться от БД.
     *  @param nameDB Имя БД.
     */
    void disconnectFromDB(QString nameDb = "");

    /*!
     * @brief Метод возвращает последнюю ошибку БД.
     */
    QSqlError getLastError();

    /*!
     *  @brief Получить данные о рейсах в аэропорт.
     *  @param airportCode Код аэропорта.
     *  @param date Дата.
     */
    void getDataArrivals(const QString& airportCode, const QString& date);

    /*!
     *  @brief Получить данные о рейсах из аэропорта.
     *  @param airportCode Код аэропорта.
     *  @param date Дата.
     */
    void getDataDepartures(const QString& airportCode, const QString& date);

    /*!
     *  @brief Получить данные за год по месяцам.
     *  @param airportCode Код аэропорта.
     */
    void getDataPerYear(const QString &airportCode);

    /*!
     *  @brief Получить данные за год по дням.
     *  @param airportCode Код аэропорта.
     */
    void getDataPerMonth(const QString &airportCode);

signals:

    /*!
     * @brief Сигнал: "Отправить статус подключения".
     */
    void sig_SendStatusConnection(bool statusConnection_);

    /*!
     * @brief Сигнал: "Отправить список аэропортов".
     * @param model Модель SQL запроса.
     */
    void sig_SendListAirports(QSqlQueryModel* model);

    /*!
     * @brief Сигнал: "Отправить модель SQL запроса".
     * @param model Модель SQL запроса.
     */
    void sig_SendQueryFromDB(QSqlQueryModel* model);

    /*!
     * @brief Сигнал: "Отправить данные о рейсах в аэропорт".
     * @param model Модель SQL запроса.
     */
    void sig_SendDataToArrivals(QSqlQueryModel* model);

    /*!
     * @brief Сигнал: "Отправить данные о рейсах из аэропорта".
     * @param model Модель SQL запроса.
     */
    void sig_SendDataToDepartures(QSqlQueryModel* model);

    /*!
     * @brief Сигнал: "Отправить данные за год по месяцам".
     * @param model Модель SQL запроса.
     */
    void sig_SendDataPerYear(QSqlQueryModel* model);

    /*!
     * @brief Сигнал: "Отправить данные за год по дням".
     * @param model Модель SQL запроса.
     */
    void sig_SendDataPerMonth(QSqlQueryModel* model);

private:
    QSqlDatabase *dataBase_; //!< База данных.
    bool statusConnection_; //!< Статус подключения.
    QSqlQueryModel* modelQueryMain_; //!< Модель SQL запроса для главной формы.
    QSqlQueryModel* modelQueryStatistics_; //!< База данных для статистики загрузки аэропорта.

    QString convertInputDate(const QString& date); //!< Привести дату в нужный формат.



};

#endif // DATABASE_H
