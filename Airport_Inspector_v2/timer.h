#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QTime>

static const int INTERVAL = 5000;

class Timer : public QObject
{
    Q_OBJECT

public:
    Timer(QObject *parent = nullptr);
    ~Timer();
    /*!
     *  @brief Запуск таймера
     */
    void StartTimer();

signals:
    /*!
     *  @brief Сигнал реконнекта
     */
    void sig_Reconnect();

private:
    QTimer *timer;
    /*!
     *  @brief Попытка реконнекта по истечению таймера
     */
    void TimeToReconnect();
};

#endif // TIMER_H
