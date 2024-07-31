#ifndef AIRPORTSTATISTIC_H
#define AIRPORTSTATISTIC_H

#include <QWidget>
#include <QtCharts>

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
    QString name = "";
    QMap<int, QString> months;

    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;

    void InitialSetup();
};

#endif // AIRPORTSTATISTIC_H
