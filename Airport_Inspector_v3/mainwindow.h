#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QSortFilterProxyModel>
#include <QDateEdit>
#include <QDate>
#include <QStandardItemModel>
#include "database.h"
#include "airportsloadwindow.h"

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
    void ReceiveStatusConnectionToDB(bool status);
    void PlotWindowDestroyed();

private slots:

    void on_pb_requestStats_clicked();

    void on_dt_dateEnter_userDateChanged(const QDate &date);

    void on_pb_request_shedule_clicked();

    void reconnectToDatabase();

    void connectToDatabase();

signals:
    void YearStatsReady(QStandardItemModel* YearStats);
    void PerDayStatsReady(QStandardItemModel* PerDayStats);
    void AirportName(QString AirPortName);

private:
    Ui::MainWindow *ui;
    DataBase* dataBase;
    QMessageBox* msg;
    QVector<QString> dataForConnect;
    const QString AirpotsList_req = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data";

    const QString ArrivalPlains_req = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                                      "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport ";
                                      //"where f.arrival_airport  = ‘airportCode'";
    const QString LivedPlains_req = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
                                    "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport ";
                                    //"WHERE f.departure_airport  = ‘airportCode’";

    const QString YearStats_req = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
                                    "WHERE (scheduled_departure::date > date('2016-08-31') and "
                                    "scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = ";
                                    //"'airportCode' or arrival_airport = 'airportCode' ) "
                                    //"group by \"Month\"";

    const QString PerDayStats_req = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
                                        "WHERE(scheduled_departure::date > date('2016-08-31') and "
                                        "scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = ";
                                    //    "airportCode or arrival_airport = airportCode) "
                                    //    "GROUP BY \"Day\"";

    QSqlQueryModel* db_answer;
    QSortFilterProxyModel *proxyModel;
    QMap<QString, QString> airportMap;
    QString request;
    const QDate minDate{2016, 8, 15}; // 2016-08-15
    const QDate maxDate{2017, 9, 14}; // 2017-09-14

    QDate CurrentDate;

    QTimer *reconnectTimer;

    QStandardItemModel* model;

    QStandardItemModel* YearStats;

    QStandardItemModel* PerDayStats;

    AirportsLoadWindow* PlotWindow;



};
#endif // MAINWINDOW_H
