#include "airportstatistic.h"
#include "ui_airportstatistic.h"

AirportStatistic::AirportStatistic(QWidget *parent, QString name_) : QWidget(parent), name(name_), ui(new Ui::AirportStatistic) {
    ui->setupUi(this);
    InitialSetup();
}

AirportStatistic::~AirportStatistic() {
    delete ui;
    delete year_layout;
    delete month_layout;
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

    year_layout = new QGridLayout;
    month_layout = new QGridLayout;

    for (size_t i = 0; i < 100; i++){
        SetYearGraph(i);
        SetMonthGraph(i);
    }
}

void AirportStatistic::ClearLayout(QGridLayout *layout){
    if (layout == nullptr) {
        return;
    }

    while (layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);
        if (item->widget()){
            delete item->widget();
        }
        delete item;
    }
}

void AirportStatistic::SetYearGraph(size_t data) {
    ClearLayout(year_layout);
    QChart* chart = new QChart();
    QChartView* chartView = new QChartView(chart);
    QLineSeries* series = new QLineSeries(this);

    for (size_t i = 0; i < data; i++){
        *series << QPoint(i + 1, i);
    }

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

void AirportStatistic::SetMonthGraph(size_t data) {
    ClearLayout(month_layout);
    QChart* chart = new QChart();
    QChartView* chartView = new QChartView(chart);
    QLineSeries* series = new QLineSeries(this);

    for (size_t i = 0; i < data; i++){
        *series << QPoint(i + 1, i);
    }

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


void AirportStatistic::on_cb_Months_currentIndexChanged(int index) {

}

