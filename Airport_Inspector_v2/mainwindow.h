#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QMessageBox>
#include "database.h"
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /*!
     * @brief Получить статус подключения к БД.
     */
    void rcv_StatusConnectionToDB(bool status);
    /*!
     * @brief Получить статус реквеста к БД.
     */
    void rcv_StatusRequest(QSqlError err);
    /*!
     * @brief Получить данные из БД.
     */
    void rcv_Airports(QSqlQueryModel* model);
    /*!
     * @brief Отображение запроса к БД в главном окне.
     * @param model Модель SQL запроса.
     */
    void rcv_QueryFromDB(QSqlQueryModel *model);
    /*!
     * @brief Данные о загрзке аэропорта за год.
     * @param model Модель SQL запроса.
     */
    void rcv_DataPerYear(QSqlQueryModel *model);
    /*!
     * @brief Данные о загрзке аэропорта за месяц.
     * @param model Модель SQL запроса.
     */
    void rcv_DataPerMonth(QSqlQueryModel *model);

signals:
    /*!
     * @brief Запуск таймера до реконнекта.
     */
    void sig_StartTimer();

private slots:
    void on_pb_GetShedule_clicked();

private:
    ///!< Главное окно.
    Ui::MainWindow *ui;
    ///!< БД.
    Database* data_base;
    //!< Таймер до попытки реконнекта.
    Timer* timer;
    //!< Окно ошибки подключения к БД.
    QMessageBox *msg;
    //!< Аэропорты.
    QMap<QString, QString> airports;

    /*!
     * @brief Подключение к БД.
     */
    void ConnectToDB();

    /*!
     * @brief Первоначальная настройка окна.
     */
    void InitialSetup();
    /*!
     * @brief Запрос данных обо всех аэропортах.
     */
    void GetAirports();
};
#endif // MAINWINDOW_H
