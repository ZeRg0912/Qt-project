#ifndef AIRPORTSTATISTIC_H
#define AIRPORTSTATISTIC_H

#include <QWidget>
#include <QtCharts>
#include <QSqlQueryModel>

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

public slots:
    void rcv_DataPerYear(QSqlQueryModel* model);
    void rcv_DataPerMonth(QSqlQueryModel* model);

private slots:
    void on_pb_Close_clicked();
    void on_cb_Months_highlighted(int index);

private:
    Ui::AirportStatistic *ui;
    QString name = "";
    QMap<int, QString> months;

    QChart* year_chart;
    QChartView* year_chart_view;
    QLineSeries* year_series;
    QVector<QPointF>* year_data;

    QChart* month_chart;
    QChartView* month_chart_view;
    QLineSeries* month_series;
    QVector<QVector<QPointF>>* months_data;

    void InitialSetup();
    void ChartsSetup();
    void UpdateYearGraph(const QVector<QPointF>& data, QChartView* chart_view);
    void UpdateMonthGraph(const QVector<QPointF>& data, QChartView* chart_view);

    QVector<QString> month_names = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
};

#endif // AIRPORTSTATISTIC_H
