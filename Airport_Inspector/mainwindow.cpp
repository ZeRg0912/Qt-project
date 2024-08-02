#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    data_base = new Database(this);
    timer = new Timer(this);
    msg = new QMessageBox(this);
    statistic = new AirportStatistic();

    InitialSetup();

    connect(data_base, &Database::sig_SendStatusConnection, this, &MainWindow::rcv_StatusConnectionToDB);
    connect(data_base, &Database::sig_SendStatusRequest,    this, &MainWindow::rcv_StatusRequest);
    connect(data_base, &Database::sig_SendAirports,         this, &MainWindow::rcv_Airports);
    connect(data_base, &Database::sig_SendArrivals,         this, &MainWindow::rcv_ArrivalDeparture);
    connect(data_base, &Database::sig_SendDepartures,       this, &MainWindow::rcv_ArrivalDeparture);
    connect(data_base, &Database::sig_SendDataPerYear,      statistic, &AirportStatistic::rcv_DataPerYear);
    connect(data_base, &Database::sig_SendDataPerMonth,     statistic, &AirportStatistic::rcv_DataPerMonth);

    connect(timer, &Timer::sig_Reconnect, this, &MainWindow::ConnectToDB);
    connect(this, &MainWindow::sig_StartTimer, timer, &Timer::StartTimer);
}

MainWindow::~MainWindow() {
    delete ui;
    delete statistic;
}

//!< Main methods
void MainWindow::ConnectToDB() {
    auto connect_to_DB = [this]{
        ui->lb_StatusConnect->setText("Подключение...");
        ui->lb_StatusConnect->setStyleSheet("color:black");
        data_base->ConnectToDB();
    };
    auto future = QtConcurrent::run(connect_to_DB);
}

void MainWindow::rcv_StatusConnectionToDB(bool status_connection) {
    if(status_connection) {
        ui->lb_StatusConnect->setText("Подключено к БД");
        ui->lb_StatusConnect->setStyleSheet("color:green");
        ui->gb_ControlPanel->setEnabled(true);
        GetAirports();
    } else {
        data_base->DisconnectFromDataBase(DB_NAME);
        ui->lb_StatusConnect->setText("Не удалось подключиться к БД!");
        ui->lb_StatusConnect->setStyleSheet("color:red");
        msg->setIcon(QMessageBox::Critical);
        msg->setText(data_base->GetLastError().text());
        msg->exec();
        emit sig_StartTimer();
    }
}

void MainWindow::rcv_StatusRequest(QSqlError err) {
    if (err.type() != QSqlError::NoError) {
        msg->setIcon(QMessageBox::Warning);
        msg->setText(err.text());
        msg->exec();
    }
}

void MainWindow::rcv_Airports(QSqlQueryModel* model) {
    for (size_t i = 0; i < model->rowCount(); i++) {
        ui->cb_Airports->addItem(model->data(model->index(i, 0)).toString());
        airports[model->data(model->index(i, 0)).toString()] = model->data(model->index(i, 1)).toString();
    }

    ui->tv_MainWindow->setModel(model);
}

void MainWindow::rcv_ArrivalDeparture(QSqlQueryModel *model) {
    ui->tv_MainWindow->setModel(model);
}

//!< UTILS
void MainWindow::InitialSetup() {
    ui->statusbar->addWidget(ui->lb_StatusConnect);
    ui->lb_StatusConnect->setText("Не подключен к БД");
    ui->lb_StatusConnect->setStyleSheet("color:red");

    ui->lb_Date->setText("Дата:");
    ui->rb_Arrival->setText("Прибытие");
    ui->rb_Departure->setText("Вылет");
    ui->pb_GetShedule->setText("Получить расписание");
    ui->pb_ShowWorkload->setText("Показать загруженность");

    ui->gb_ControlPanel->setEnabled(false);

    data_base->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    ConnectToDB();
}

void MainWindow::GetAirports() {    
    ui->tv_MainWindow->setModel(nullptr);
    data_base->GetAirports();
}

void MainWindow::on_pb_GetShedule_clicked() {
    QString airport_code = airports[ui->cb_Airports->currentText()];
    QString date = ui->de_Date->text();

    if (ui->rb_Arrival->isChecked()) {
        data_base->GetArrivals(airport_code, date);
    } else if (ui->rb_Departure->isChecked()) {
        data_base->GetDepartures(airport_code, date);
    }
}

void MainWindow::on_pb_ShowWorkload_clicked() {
    statistic->close();
    statistic->SetAirportName(ui->cb_Airports->currentText());
    data_base->GetDataPerYear(airports[ui->cb_Airports->currentText()]);
    data_base->GetDataPerMonth(airports[ui->cb_Airports->currentText()]);
    statistic->show();
}

