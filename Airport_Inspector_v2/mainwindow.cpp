#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    data_base = new Database(this);
    timer = new Timer(this);
    msg = new QMessageBox(this);

    ui->statusbar->addWidget(ui->lb_statusConnect);
    ui->lb_statusConnect->setText("Не подключен к БД");

    data_base->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    ConnectToDB();

    connect(data_base, &Database::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(timer, &Timer::sig_Reconnect, this, &MainWindow::ConnectToDB);
    connect(this, &MainWindow::sig_StartTimer, timer, &Timer::StartTimer);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::ConnectToDB() {
    auto connect_to_DB = [this]{
        ui->lb_statusConnect->setText("Подключение...");
        ui->lb_statusConnect->setStyleSheet("color:black");
        data_base->ConnectToDataBase();
    };
    QtConcurrent::run(connect_to_DB);
}


void MainWindow::ReceiveStatusConnectionToDB(bool status_connection) {
    if(status_connection) {
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
    }
    else {
        data_base->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Не удалось подключиться к БД!");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->setIcon(QMessageBox::Critical);
        msg->setText(data_base->GetLastError().text());
        msg->exec();
        emit sig_StartTimer();
    }
}

