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

private slots:
    void on_pb_Close_clicked();
    void on_cb_Months_currentIndexChanged(int index);

private:
    Ui::AirportStatistic *ui;
    QString name = "";
    QMap<int, QString> months;

    QChart* month_chart;
    QChartView* month_chart_view;
    QLineSeries* month_series;

    QChart* year_chart;
    QChartView* year_chart_view;
    QLineSeries* year_series;

    void InitialSetup();
    void ChartsSetup();
    void UpdateGraph(const QVector<QPointF>& data, QChartView* chart_view);
};

#endif // AIRPORTSTATISTIC_H
