#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QTableWidget>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQueryModel>

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
     *  @brief Реквест к ДБ.
     *  @param SQL запрос.
     */
    void RequestToDB(const QString& request, QTableView* table_view);

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
     * @brief Сигнал: "Отправить данные из ДБ".
     */
    void sig_SendDataFromDB(const QSqlQueryModel& model);

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
