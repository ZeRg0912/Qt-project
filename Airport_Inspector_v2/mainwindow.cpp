#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    data_base = new Database(this);
    timer = new Timer(this);
    msg = new QMessageBox(this);

    InitialSetup();

    connect(data_base, &Database::sig_SendStatusConnection, this, &MainWindow::rcv_StatusConnectionToDB);
    connect(data_base, &Database::sig_SendStatusRequest,    this, &MainWindow::rcv_StatusRequest);

    connect(timer, &Timer::sig_Reconnect, this, &MainWindow::ConnectToDB);
    connect(this, &MainWindow::sig_StartTimer, timer, &Timer::StartTimer);
}

MainWindow::~MainWindow() {
    delete ui;
}

//!< Main methods
void MainWindow::ConnectToDB() {
    auto connect_to_DB = [this]{
        ui->lb_StatusConnect->setText("Подключение...");
        ui->lb_StatusConnect->setStyleSheet("color:black");
        data_base->ConnectToDB();
    };
    QtConcurrent::run(connect_to_DB);
}

void MainWindow::rcv_StatusConnectionToDB(bool status_connection) {
    if(status_connection) {
        ui->lb_StatusConnect->setText("Подключено к БД");
        ui->lb_StatusConnect->setStyleSheet("color:green");
        ui->gb_ControlPanel->setEnabled(true);
        FirstRequest();
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
        msg->setIcon(QMessageBox::Critical);
        msg->setText(err.text());
        msg->exec();
    }
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

void MainWindow::FirstRequest() {
    QString request = "SELECT airport_name->>'ru' AS name, airport_code FROM bookings.airports_data ORDER BY name";
    ui->tv_MainWindow->setModel(nullptr);
    data_base->RequestToDB(request, ui->tv_MainWindow);
}
