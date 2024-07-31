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

    void SetYearGraph(size_t data);
    void SetMonthGraph(size_t data);
    void ClearLayout(QGridLayout* layout);

private slots:
    void on_pb_Close_clicked();

    void on_cb_Months_currentIndexChanged(int index);

private:
    Ui::AirportStatistic *ui;
    QString name = "";
    QMap<int, QString> months;

    QGridLayout* month_layout;
    QGridLayout* year_layout;

    void InitialSetup();
};

#endif // AIRPORTSTATISTIC_H
