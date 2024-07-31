#include "airportstatistic.h"
#include "ui_airportstatistic.h"

AirportStatistic::AirportStatistic(QWidget *parent, QString name_) : QWidget(parent), name(name_), ui(new Ui::AirportStatistic) {
    ui->setupUi(this);
    InitialSetup();
}

AirportStatistic::~AirportStatistic() {
    delete ui;
    delete chart;
    delete graphClass;
    delete chartView;
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
}

void AirportStatistic::on_pb_Close_clicked() {
    this->close();
}

