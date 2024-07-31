#include "airportstatistic.h"
#include "ui_airportstatistic.h"

AirportStatistic::AirportStatistic(QWidget *parent, QString name_) : QWidget(parent), name(name_), ui(new Ui::AirportStatistic) {
    ui->setupUi(this);
    InitialSetup();
}

AirportStatistic::~AirportStatistic() {
    delete ui;
}

void AirportStatistic::InitialSetup(){
    QStringList month_names = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
    for (size_t i = 1; i < 13; i++) {
        months.insert(i, month_names[i - 1]);
    }
    for (const auto& [key, value] : months.asKeyValueRange()) {
        ui->cb_Months->addItem(value);
    }

    ui->lb_Airport->setText("Загруженность аэропорта " + name);
    ui->pb_Close->setText("Закрыть статистику");
    ui->tabWidget->setTabText(0, "За год");
    ui->tabWidget->setTabText(1, "За месяц");

    SetYearGraph();
    SetMonthGraph();
}

void AirportStatistic::SetYearGraph() {
    QChart* chart = new QChart();
    QChartView* chartView = new QChartView(chart);
    QLineSeries* series = new QLineSeries(this);
    QGridLayout* year_layout = new QGridLayout;

    *series << QPointF(1.0, 1.0) << QPointF(2.0, 73.0) << QPointF(3.0, 268.0) << QPointF(4.0, 17.0)
            << QPointF(5.0, 4325.0) << QPointF(6.0, 723.0);

    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Статистика за год:");

    QValueAxis* axisX = new QValueAxis(this);
    axisX->setTitleText("Месяцы");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(series->count());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis(this);
    axisY->setTitleText("Кол-во рейсов");
    axisY->setLabelFormat("%g");
    axisY->setMinorTickCount(-1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setRenderHint(QPainter::Antialiasing);
    ui->wid_YearChart->setLayout(year_layout);
    year_layout->addWidget(chartView);
    //chartView->show();
}

void AirportStatistic::SetMonthGraph() {
    QChart* chart = new QChart();
    QChartView* chartView = new QChartView(chart);
    QLineSeries* series = new QLineSeries(this);
    QGridLayout* month_layout = new QGridLayout;

    *series << QPointF(1.0, 15000) << QPointF(2.0, 213.0) << QPointF(3.0, 10000.0) << QPointF(4.0, 17.0)
            << QPointF(5.0, 4325.0);

    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Статистика за месяц:");

    QValueAxis* axisX = new QValueAxis(this);
    axisX->setTitleText("Дни");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(series->count());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis(this);
    axisY->setTitleText("Кол-во рейсов");
    axisY->setLabelFormat("%g");
    axisY->setMinorTickCount(-1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setRenderHint(QPainter::Antialiasing);
    ui->wid_MonthChart->setLayout(month_layout);
    month_layout->addWidget(chartView);
    //chartView->show();
}

void AirportStatistic::on_pb_Close_clicked() {
    this->close();
}

