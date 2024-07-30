#ifndef AIRPORTSTATISTIC_H
#define AIRPORTSTATISTIC_H

#include <QWidget>

namespace Ui {
class AirportStatistic;
}

class AirportStatistic : public QWidget
{
    Q_OBJECT

public:
    explicit AirportStatistic(QWidget *parent = nullptr, QString name = "");
    ~AirportStatistic();

private slots:
    void on_pb_Close_clicked();

private:
    Ui::AirportStatistic *ui;
    /*!
     * @brief Первоначальная настройка окна.
     */
    void InitialSetup();
    //!< Название аэропорта.
    QString name = "";
    //!< Месяцы.
    QMap<int, QString> months;
};

#endif // AIRPORTSTATISTIC_H
