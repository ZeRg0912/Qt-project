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
    ui->lb_Airport->setText("Загруженность аэропорта " + name);
    ui->pb_Close->setText("Закрыть статистику");
    ui->tabWidget->setTabText(0, "За месяц");
    ui->tabWidget->setTabText(1, "За год");
}

void AirportStatistic::on_pb_Close_clicked() {
    this->close();
}

