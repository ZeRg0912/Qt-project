#ifndef AIRPORTSTATISTIC_H
#define AIRPORTSTATISTIC_H

#include <QWidget>
#include <QtCharts>

#define NUM_GRAPHS 2

namespace Ui {
class AirportStatistic;
}

class AirportStatistic : public QWidget
{
    Q_OBJECT

public:
    explicit AirportStatistic(QWidget *parent = nullptr, QString name = "");
    ~AirportStatistic();

    void SetYearGraph();
    void SetMonthGraph();

private slots:
    void on_pb_Close_clicked();

private:
    Ui::AirportStatistic *ui;
    QString name = "";
    QMap<int, QString> months;

    void InitialSetup();
};

#endif // AIRPORTSTATISTIC_H
