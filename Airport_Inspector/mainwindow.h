#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QMap>

#include "database.h"
#include "stopwatch.h"
#include "statistics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    /*!
     * @brief Запустить подключения к БД.
     */
    void RunConnectionToDB();

    /*!
     * @brief Получение статуса подключения к БД.
     * @param status Статус подключения.
     */
    void RcvSignalSendStatusConnectionToDB(bool status);

    /*!
     * @brief Заполнение данных об аэропортах в чекбокс.
     * @param model Модель SQL запроса.
     */
    void RcvSignalSendListAirports(QSqlQueryModel *model);

    /*!
     * @brief Нажатие на кнопку получения расписания.
     */
    void on_pb_getList_clicked();

    /*!
     * @brief Отображение запроса к БД в главном окне.
     * @param model Модель SQL запроса.
     */
    void RcvSignalSendQueryFromDB(QSqlQueryModel *model);

    /*!
     * @brief Нажатие на кнопку получения загруженности аэропорта.
     */
    void on_pb_showLoad_clicked();

    /*!
     * @brief Данные о загрзке аэропорта за год.
     * @param model Модель SQL запроса.
     */
    void RcvSignalSendDataPerYear(QSqlQueryModel *model);

    /*!
     * @brief Данные о загрзке аэропорта за месяц.
     * @param model Модель SQL запроса.
     */
    void RcvSignalSendDataPerMonth(QSqlQueryModel *model);

    /*!
     * @brief Закрытие статистики загруженности аэропорта.
     */
    void RcvSignalCloseStatistics();

private:
    Ui::MainWindow *ui; //<! Главное окно.
    DataBase* dataBase_; //<! База данных.
    Statistics *statistics_; //<! Статистика.
    QMessageBox *msgBox_; //<! Окно ошибки подключения к БД.
    QMap<QString, QString> airports_; //<! Аэропорты.
    bool isFailConnection_; //<! Ошибка подключения к БД.
    Stopwatch *sw_; //<! Секундомер.
};
#endif // MAINWINDOW_H
