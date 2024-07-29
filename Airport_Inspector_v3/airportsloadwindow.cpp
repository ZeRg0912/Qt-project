#include "airportsloadwindow.h"
#include "ui_airportsloadwindow.h"
#include <QDebug>
#include <QDateTime>>

AirportsLoadWindow::AirportsLoadWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AirportsLoadWindow)
{
    ui->setupUi(this);

    chartYear = new QChart();
    seriesYear = new QBarSeries(this);
    chartViewYear = new QChartView(chartYear);
    chartYear->addSeries(seriesYear);
    //chartYear->createDefaultAxes();

    chartMonth = new QChart();
    seriesMonth = new QLineSeries(this);
    chartViewMonth = new QChartView(chartMonth);
    chartMonth->addSeries(seriesMonth);
    //chartMonth->createDefaultAxes();


    // Создаем макеты для вкладок
    QVBoxLayout *layoutMonth = new QVBoxLayout(this);
    QVBoxLayout *layoutYear = new QVBoxLayout(this);

    layoutMonth->addWidget(chartViewMonth);
    layoutMonth->addWidget(ui->cb_monthSelect);
    //ui->cb_monthSelect->show();
    layoutYear->addWidget(chartViewYear);


    // Добавляем макеты во вкладки
    ui->tab_month->setLayout(layoutMonth);
    ui->tab_year->setLayout(layoutYear);

    chartViewMonth->show();
    chartViewYear->show();


    for(int monthIter = 0; monthIter < 12; ++monthIter){
        QString month;
        switch (monthIter) {
        case 0:
            month = "Январь";
            break;
        case 1:
            month = "Февраль";
            break;
        case 2:
            month = "Март";
            break;
        case 3:
            month = "Апрель";
            break;
        case 4:
            month = "Май";
            break;
        case 5:
            month = "Июнь";
            break;
        case 6:
            month = "Июль";
            break;
        case 7:
            month = "Август";
            break;
        case 8:
            month = "Сентябрь";
            break;
        case 9:
            month = "Октябрь";
            break;
        case 10:
            month = "Ноябрь";
            break;
        case 11:
            month = "Декабрь";
            break;
        default:
            break;
        }
        ui->cb_monthSelect->addItem(QString(month).arg(monthIter));
    }
    PerDayStatsAllMonthReceived = false;



}

AirportsLoadWindow::~AirportsLoadWindow()
{
    delete chartYear;
    //delete chartViewYear;
    //delete chartViewMonth;
    delete chartMonth;
    delete ui;
}


void AirportsLoadWindow::on_pb_close_clicked()
{
    close();
}

void  AirportsLoadWindow::closeEvent(QCloseEvent *event)
{
    ui->cb_monthSelect->setCurrentIndex(0);
    emit destroyed();
}

void AirportsLoadWindow::ReceiveYearStats(QStandardItemModel* YearStats)
{
    seriesYear->clear();
    int rangeY = 0;

    for (int row = 0; row < YearStats->rowCount(); ++row) {

        QModelIndex index_val = YearStats->index(row, 0);
        QModelIndex index_month = YearStats->index(row, 1);

        QVariant data = YearStats->data(index_val);
        int value = data.toInt();
        if (value >rangeY) {
            rangeY = value;
        }

        QVariant QVarDate = YearStats->data(index_month);
        QDateTime dateTime = QVarDate.toDateTime();
        QDate date = dateTime.date();
        int month = date.month();
        int year = date.year();
        QString monthString = QString("%1.%2").arg(month).arg(year);

        QBarSet *set = new QBarSet(monthString);
        *set << value;
        seriesYear->append(set);
        //qDebug() << value;

    }


   axisX_year = new QValueAxis(this);
   axisY_year = new QValueAxis(this);

   axisX_year->setRange(-0.5, 0.5);
   axisX_year->setTitleText("Месяц");
   axisX_year->setLabelFormat("%d"); // Формат отображения меток оси X

   axisY_year->setRange(0, rangeY);
   axisY_year->setTitleText("Количество рейсов");

   chartYear->addAxis(axisX_year, Qt::AlignBottom);
   chartYear->addAxis(axisY_year, Qt::AlignLeft);
   seriesYear->attachAxis(axisX_year);
   seriesYear->attachAxis(axisY_year);
   chartViewYear->update();
   chartYear->createDefaultAxes();

}
void AirportsLoadWindow::ReceivePerDayStats(QStandardItemModel* PerDayStats)
{
    PerDayStatsAllMonth = PerDayStats;
    PerDayStatsAllMonthReceived = true;
    AirportsLoadWindow::plotPerDayStats();
}

void AirportsLoadWindow::plotPerDayStats()
{
    seriesMonth->clear();
    int rangeYmax = 0;
    int rangeYmin = 0;
    int dayCount = 0;
    int monthIndex = ui->cb_monthSelect->currentIndex() + 1;
    for (int row = 0; row < PerDayStatsAllMonth->rowCount(); ++row) {

        QModelIndex index_val = PerDayStatsAllMonth->index(row, 0);
        QModelIndex index_month = PerDayStatsAllMonth->index(row, 1);

        QVariant data = PerDayStatsAllMonth->data(index_val);
        int value = data.toInt();
        //qDebug() << value;
        rangeYmin = value;
        if (value > rangeYmax) {
            rangeYmax = value;
        }
        if (value < rangeYmin) {
            rangeYmin = value;
        }
        QVariant QVarDate = PerDayStatsAllMonth->data(index_month);
        QDateTime dateTime = QVarDate.toDateTime();
        QDate date = dateTime.date();
        int month = date.month();

        if (month == monthIndex)
        {
            dayCount++;
            seriesMonth->append(dayCount, value);
        }
    }
    axisX_month = new QValueAxis(this);
    axisY_month = new QValueAxis(this);

    axisX_month->setRange(1, dayCount);
    //axisX_month->setTickCount(dayCount - 1);
    axisX_month->setTitleText("День месяца"); //Не видно...
    //axisX_month->setLabelFormat("%d"); // Формат отображения меток оси X

    axisY_month->setRange(std::round(rangeYmin * 0.9 - 1), 1 + std::round(rangeYmax * 1.05));
    //axisY_month->setTickCount(rangeYmax - rangeYmin);
    axisY_month->setTitleText("Количество рейсов"); //Не видно...

    chartMonth->addAxis(axisX_month, Qt::AlignBottom);
    chartMonth->addAxis(axisY_month, Qt::AlignLeft);
    seriesMonth->attachAxis(axisX_month);
    seriesMonth->attachAxis(axisY_month);
    chartViewMonth->update();
    chartMonth->createDefaultAxes();
}

void AirportsLoadWindow::ReceiveAirportName(QString AirportName)
{
    ui->tl_AirportName->setText("Статистика вылетов и прилетов для аэропорта: " + AirportName);
}

void AirportsLoadWindow::on_cb_monthSelect_currentIndexChanged(int index)
{
    if (PerDayStatsAllMonthReceived == true) {
        AirportsLoadWindow::plotPerDayStats();
    }
}

