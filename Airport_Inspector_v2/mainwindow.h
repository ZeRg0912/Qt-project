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
    void ReceiveStatusConnectionToDB(bool status);

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
};
#endif // MAINWINDOW_H
