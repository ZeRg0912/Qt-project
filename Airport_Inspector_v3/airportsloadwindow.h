#ifndef AIRPORTSLOADWINDOW_H
#define AIRPORTSLOADWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QtCharts>

namespace Ui {
class AirportsLoadWindow;
}

class AirportsLoadWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AirportsLoadWindow(QWidget *parent = nullptr);
    ~AirportsLoadWindow();

signals:
    //void PlotWindowclosed();
private slots:
    void on_pb_close_clicked();

    void on_cb_monthSelect_currentIndexChanged(int index);

public slots:
    void ReceiveYearStats(QStandardItemModel* YearStats);
    void ReceivePerDayStats(QStandardItemModel* PerDayStats);
    void ReceiveAirportName(QString AirportName);


private:
    Ui::AirportsLoadWindow *ui;
    QChart *chartYear;
    QChartView *chartViewYear;
    QBarSeries *seriesYear;

    QChart *chartMonth;
    QChartView *chartViewMonth;
    QLineSeries *seriesMonth;

    QValueAxis *axisX;
    QValueAxis *axisY;

    QValueAxis *axisX_year;
    QValueAxis *axisY_year;

    QValueAxis *axisX_month;
    QValueAxis *axisY_month;

    QStandardItemModel* PerDayStatsAllMonth;

    bool PerDayStatsAllMonthReceived;

    void plotPerDayStats();

protected:
    void closeEvent(QCloseEvent *event) override;
};


#endif // AIRPORTSLOADWINDOW_H
