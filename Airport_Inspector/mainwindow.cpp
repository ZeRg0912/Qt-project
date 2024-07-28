#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tb_main->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_main->verticalHeader()->setVisible(false);
    ui->groupBox->setEnabled(false);
    ui->lb_status->setText("Отключено от БД");
    ui->lb_status->setStyleSheet("color:red");
    ui->statusbar->addWidget(ui->lb_status);
    ui->rb_arrival->setChecked(true);
    ui->centralwidget->setWindowTitle("Расписание");

    dataBase_ = new DataBase(this);
    dataBase_->addDataBase(POSTGRE_DRIVER, DB_NAME);

    statistics_ = new Statistics();

    msgBox_ = new QMessageBox(this);

    isFailConnection_ = false;

    sw_ = new Stopwatch(this);
    sw_->Start();

    connect(sw_->getQTimer(), &QTimer::timeout, this, &MainWindow::RunConnectionToDB);

    connect(dataBase_, &DataBase::sig_SendStatusConnection, this, &MainWindow::RcvSignalSendStatusConnectionToDB);
    connect(dataBase_, &DataBase::sig_SendListAirports, this, &MainWindow::RcvSignalSendListAirports);
    connect(dataBase_, &DataBase::sig_SendQueryFromDB, this, &MainWindow::RcvSignalSendQueryFromDB);
    connect(dataBase_, &DataBase::sig_SendDataToArrivals, this, &MainWindow::RcvSignalSendQueryFromDB);
    connect(dataBase_, &DataBase::sig_SendDataToDepartures, this, &MainWindow::RcvSignalSendQueryFromDB);
    connect(dataBase_, &DataBase::sig_SendDataPerYear, this, &MainWindow::RcvSignalSendDataPerYear);
    connect(dataBase_, &DataBase::sig_SendDataPerMonth, this, &MainWindow::RcvSignalSendDataPerMonth);
    connect(statistics_, &Statistics::sig_CloseStatistics, this, &MainWindow::RcvSignalCloseStatistics);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RunConnectionToDB()
{
    if (!dataBase_->getStatusConnection() && !isFailConnection_)
    {
        auto conn = [&]{dataBase_->connectToDB();};
        QtConcurrent::run(conn);

    }
    if (isFailConnection_)
    {
        sw_->setTime(sw_->getCurrentTime() + 1);
        if(sw_->getCurrentTime() >= 5)
        {
            isFailConnection_ = !isFailConnection_;
        }
    }
}

void MainWindow::RcvSignalSendStatusConnectionToDB(bool status)
{
    if (status)
    {
        ui->lb_status->setText("Подключено к БД");
        ui->lb_status->setStyleSheet("color:green");
        ui->statusbar->addWidget(ui->lb_status);
        ui->groupBox->setEnabled(true);
        sw_->Stop();
    }
    else
    {
        isFailConnection_ = !isFailConnection_;
        ui->lb_status->setText("Отключено от БД");
        ui->lb_status->setStyleSheet("color:red");
        ui->statusbar->addWidget(ui->lb_status);
        dataBase_->disconnectFromDB(DB_NAME);
        msgBox_->setIcon(QMessageBox::Critical);
        msgBox_->setText(dataBase_->getLastError().text());
        sw_->Stop();
        msgBox_->exec();
        sw_->setTime(0);
        sw_->Start();
    }
}

void MainWindow::RcvSignalSendListAirports(QSqlQueryModel *model)
{
    for(int i = 0; i < model->rowCount(); ++i)
    {
        ui->cb_airports->addItem(model->data(model->index(i,0)).toString());
        airports_[model->data(model->index(i,0)).toString()] = model->data(model->index(i,1)).toString();
    }

}

void MainWindow::on_pb_getList_clicked()
{
    QString airportCode = airports_[ui->cb_airports->currentText()];
    QString date = ui->de_date->text();
    if(ui->rb_arrival->isChecked())
    {
        dataBase_->getDataArrivals(airportCode, date);
    }
    else if(ui->rb_departure->isChecked())
    {
        dataBase_->getDataDepartures(airportCode, date);
    }
}

void MainWindow::RcvSignalSendQueryFromDB(QSqlQueryModel *model)
{
    ui->tb_main->setModel(model);
}

void MainWindow::on_pb_showLoad_clicked()
{
    ui->centralwidget->setEnabled(false);
    QString airportCode = ui->cb_airports->currentText() +
            " (" + airports_[ui->cb_airports->currentText()] + ")";
    statistics_->setAirportText(airportCode);
    dataBase_->getDataPerYear(airports_[ui->cb_airports->currentText()]);
    dataBase_->getDataPerMonth(airports_[ui->cb_airports->currentText()]);
    statistics_->show();
}

void MainWindow::RcvSignalSendDataPerYear(QSqlQueryModel *model)
{
    QVector<double> data;
    for(int i = 0; i < model->rowCount(); ++i)
    {
        data.append(model->data(model->index(i,0)).toDouble());
    }
    statistics_->setDataPerYear(data);
}

void MainWindow::RcvSignalSendDataPerMonth(QSqlQueryModel *model)
{
    QMultiMap<int, double> data;
    for(int i = 0; i < model->rowCount(); ++i)
    {
        data.insert(model->data(model->index(i,1)).toInt(), model->data(model->index(i,0)).toDouble());
    }
    statistics_->setDataPerMonth(data);
}

void MainWindow::RcvSignalCloseStatistics()
{
    ui->centralwidget->setEnabled(true);
    statistics_->close();
}



