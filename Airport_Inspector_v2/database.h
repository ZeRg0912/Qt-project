#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "test-db"
#define NUM_DATA_FOR_CONNECT_TO_DB 5

class Database : public QObject
{
    Q_OBJECT

public:
    /*!
     *  @brief Перечисление полей данных.
     */
    enum fieldsForConnect{
        hostName = 0,
        dbName = 1,
        login = 2,
        pass = 3,
        port = 4
    };

    Database(QObject *parent = nullptr);
    ~Database();

    /*!
     *  @brief Добавить БД.
     *  @param driver Драйвер БД.
     *  @param nameDB Имя БД.
     */
    void AddDataBase(QString driver, QString nameDB = "");

    /*!
     * @brief Возвращает последнюю ошибку БД.
     */
    QSqlError GetLastError(void);

    /*!
     *  @brief Подключиться к БД.
     */
    void ConnectToDataBase();

    /*!
     *  @brief Отключиться от БД.
     *  @param nameDB Имя БД.
     */
    void DisconnectFromDataBase(QString nameDb = "");

    /*!
     *  @brief Получить данные о рейсах в аэропорт.
     *  @param airport_code Код аэропорта.
     *  @param date Дата.
     */
    void GetDataArrivals(const QString& airport_code, const QString& date);

    /*!
     *  @brief Получить данные о рейсах из аэропорта.
     *  @param airport_code Код аэропорта.
     *  @param date Дата.
     */
    void GetDataDepartures(const QString& airport_code, const QString& date);

    /*!
     *  @brief Получить данные за год по месяцам.
     *  @param airport_code Код аэропорта.
     */
    void GetDataPerYear(const QString &airport_code);

    /*!
     *  @brief Получить данные за год по дням.
     *  @param airport_code Код аэропорта.
     */
    void GetDataPerMonth(const QString &airport_code);

signals:
    /*!
     * @brief Сигнал: "Отправить статус подключения".
     */
    void sig_SendStatusConnection(bool status_connection);

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
    //!< База данных
    QSqlDatabase* data_base;
    //!< Статус подключения.
    bool status_connection;
    //!< Модель SQL запроса для главной формы.
    QSqlQueryModel* model_main;
     //!< Модель SQL запроса для статистики загрузки аэропорта.
    QSqlQueryModel* model_statistic;
     //!< Конвертация даты в нужный формат.
    QString ConvertDate(const QString& date);
};

#endif // DATABASE_H
