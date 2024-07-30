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

signals:
    /*!
     * @brief Запуск таймера до реконнекта.
     */
    void sig_StartTimer();

private:
    ///!< Главное окно.
    Ui::MainWindow *ui;
    ///!< БД.
    Database* data_base;
    //!< Таймер до попытки реконнекта.
    Timer* timer;
    //!< Окно ошибки подключения к БД.
    QMessageBox *msg;

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
    void FirstRequest();
};
#endif // MAINWINDOW_H
