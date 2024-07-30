#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QTableWidget>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQueryModel>
#include <QtConcurrent>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "test-db"

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
    void ConnectToDB();
    /*!
     *  @brief Отключиться от БД.
     *  @param nameDB Имя БД.
     */
    void DisconnectFromDataBase(QString nameDb = "");
    /*!
     *  @brief Реквест получения списка аэропортов к ДБ.
     *  @param SQL запрос.
     */
    void GetAirports(const QString& request);    /*!
     *  @brief Получить данные о рейсах в аэропорт.
     *  @param airport_code Код аэропорта.
     *  @param date Дата.
     */
    void GetArrivals(const QString& airport_code, const QString& date);
    /*!
     *  @brief Получить данные о рейсах из аэропорта.
     *  @param airport_code Код аэропорта.
     *  @param date Дата.
     */
    void GetDepartures(const QString& airport_code, const QString& date);
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
     * @brief Сигнал: "Отправить статус подключения".
     */
    void sig_SendStatusRequest(QSqlError err);
    /*!
     * @brief Сигнал: "Отправить данные об аэропортах из ДБ".
     * @param model Модель SQL запроса.
     */
    void sig_SendAirports(QSqlQueryModel* model);
    /*!
     * @brief Сигнал: "Отправить данные о рейсах в аэропорт".
     * @param model Модель SQL запроса.
     */
    void sig_SendArrivals(QSqlQueryModel* model);
    /*!
     * @brief Сигнал: "Отправить данные о рейсах из аэропорта".
     * @param model Модель SQL запроса.
     */
    void sig_SendDepartures(QSqlQueryModel* model);
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
    //!< Модель SQL запроса.
    QSqlQueryModel* query_model;
    //!< Конвертация даты в нужный формат.
    QString ConvertDate(const QString& date);
};

#endif // DATABASE_H
