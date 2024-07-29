#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PlotWindow = new AirportsLoadWindow();

    ui->lb_statusConnect->setStyleSheet("color:red");

    reconnectTimer = new QTimer(this);
    reconnectTimer->setSingleShot(true); // Таймер выполнится только один раз
    connect(reconnectTimer, &QTimer::timeout, this, &MainWindow::reconnectToDatabase);

    dataBase = new DataBase(this);
    msg = new QMessageBox(this);
    db_answer = new  QSqlQueryModel(this);
    proxyModel  = new  QSortFilterProxyModel(this);
    QDate CurrentDate {2017, 1, 1};
    ui->dt_dateEnter->setDate(CurrentDate);
    ui->rb_arrival->setChecked(true);
    ui->rb_departure->setChecked(false);
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(PlotWindow, &AirportsLoadWindow::destroyed, this, &MainWindow::PlotWindowDestroyed); //ToDO: не работает при закрытии крестиком

    connect(this, &MainWindow::AirportName, PlotWindow, &AirportsLoadWindow::ReceiveAirportName);

    connect(this, &MainWindow::YearStatsReady, PlotWindow, &AirportsLoadWindow::ReceiveYearStats);

    connect(this, &MainWindow::PerDayStatsReady, PlotWindow, &AirportsLoadWindow::ReceivePerDayStats);

    connectToDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete PlotWindow;
}

void MainWindow::PlotWindowDestroyed()
{
    //qDebug() << "PlotWindow destr.";
    setDisabled(false);
}

void MainWindow::connectToDatabase()
{
    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");

       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        //ui->pb_request_shedule->setEnabled(false);
    }
}


void MainWindow::reconnectToDatabase()
{
    connectToDatabase();
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->centralwidget->setEnabled(true);
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        db_answer = dataBase->RequestToDB(AirpotsList_req);
        proxyModel->setSourceModel(db_answer);
        // Устанавливаем сортировку по определенному столбцу (например, столбец с индексом 1) по возрастанию
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive); // Настройка чувствительности к регистру
        proxyModel->setSortRole(Qt::DisplayRole); // Роль данных для сортировки
        proxyModel->sort(0, Qt::AscendingOrder); // Сортировка по столбцу 1 в возрастающем порядке

        //ui->cb_AirportsList->setModel(proxyModel);
        //ui->cb_AirportsList->setModelColumn(0);
        //ui->pb_request_shedule->setEnabled(true);
        int rowCount = db_answer->rowCount();
        for (int row = 0; row < rowCount; ++row) {
            QModelIndex codeIndex = db_answer->index(row, 0); // Индекс столбца с кодом аэропорта
            QModelIndex nameIndex = db_answer->index(row, 1); // Индекс столбца с названием аэропорта

            QString airportCode = db_answer->data(codeIndex).toString();
            QString airportName = db_answer->data(nameIndex).toString();

            airportMap.insert(airportCode, airportName);
        }
        for (const QString &airportName : airportMap.keys()) {
            ui->cb_AirportsList->addItem(airportName);
        }
    }
    else{
        ui->centralwidget->setEnabled(false);
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
        ui->cb_AirportsList->setModel(nullptr);

        // Запустить таймер на повторное подключение через 5 секунд
        reconnectTimer->start(5000);

    }

}


void MainWindow::on_pb_requestStats_clicked()
{
    setDisabled(true);
    PlotWindow->show();
    QString keyToSearch = ui->cb_AirportsList->currentText();
    if (airportMap.contains(keyToSearch)) {
        QString airportName = airportMap.value(keyToSearch);
        //ui->tl_AirportCode->setText(airportName);
        //qDebug() << "Airport name for code" << keyToSearch << "is" << airportName;
    } else {
        //qDebug() << "Airport code" << keyToSearch << "not found in the map.";
    }
    QString airportCode = airportMap.value(keyToSearch);
    request = YearStats_req + "'" + airportCode + "'" +" or arrival_airport = '" + airportCode +"') " + "group by \"Month\"";

    db_answer = dataBase->RequestToDB(request);
    YearStats = new QStandardItemModel(this);
    int rowCount = db_answer->rowCount();
    int columnCount = db_answer->columnCount();
    for (int row = 0; row < rowCount; ++row) {
        QList<QStandardItem*> rowItems;
        for (int column = 0; column < columnCount; ++column) {
            QVariant cellData = db_answer->data(db_answer->index(row, column));
            QStandardItem *item = new QStandardItem();
            item->setData(cellData, Qt::DisplayRole);
            rowItems.append(item);
        }
        YearStats->appendRow(rowItems);
    }

    request = PerDayStats_req + "'" + airportCode + "'" +" or arrival_airport = '" + airportCode +"') " + "group by \"Day\"";
    db_answer = dataBase->RequestToDB(request);
    PerDayStats = new QStandardItemModel(this);
    rowCount = db_answer->rowCount();
    columnCount = db_answer->columnCount();
    for (int row = 0; row < rowCount; ++row) {
        QList<QStandardItem*> rowItems;
        for (int column = 0; column < columnCount; ++column) {
            QVariant cellData = db_answer->data(db_answer->index(row, column));
            QStandardItem *item = new QStandardItem();
            item->setData(cellData, Qt::DisplayRole);
            rowItems.append(item);
        }
        PerDayStats->appendRow(rowItems);
    }
    //ui->tv_AirPortsTable->setModel(PerDayStats);
    emit AirportName(ui->cb_AirportsList->currentText());
    emit YearStatsReady(YearStats);
    emit PerDayStatsReady(PerDayStats);
}


void MainWindow::on_dt_dateEnter_userDateChanged(const QDate &date)
{
    CurrentDate = ui->dt_dateEnter->date();
    if (CurrentDate < minDate){
        CurrentDate = minDate;
        ui->dt_dateEnter->setDate(CurrentDate);
    }
    if (CurrentDate > maxDate){
        CurrentDate = maxDate;
        ui->dt_dateEnter->setDate(CurrentDate);
    }
}


void MainWindow::on_pb_request_shedule_clicked()
{
    QString keyToSearch = ui->cb_AirportsList->currentText();
    QString airportCode = airportMap.value(keyToSearch);
    if (airportMap.contains(keyToSearch)) {
        //ui->tl_AirportCode->setText(airportName);
        //qDebug() << "Airport name for code" << keyToSearch << "is" << airportCode;
    } else {
        //qDebug() << "Airport code" << keyToSearch << "not found in the map.";
    }
    if (ui->rb_arrival->isChecked() == true){
        request = ArrivalPlains_req + "where f.arrival_airport  = " + "'" +  airportCode + "' "
                "and scheduled_departure::date = date('" + CurrentDate.toString() + "')";
    }
    else if (ui->rb_departure->isChecked() == true){
        request = LivedPlains_req + "where f.departure_airport = " + "'" +  airportCode + "' "
                "and scheduled_departure::date = date('" + CurrentDate.toString() + "')";
    }
    else {
        request = "";
    }

    db_answer = dataBase->RequestToDB(request);

    //Данные из базы данных будем хранить в QStandardItemModel, т.к. нужен тип QVAriant
    model = new QStandardItemModel(this);

    //Заполним таблицу значениямииз БД
    int rowCount = db_answer->rowCount();
    int columnCount = db_answer->columnCount();
    for (int row = 0; row < rowCount; ++row) {
        QList<QStandardItem*> rowItems;
        for (int column = 0; column < columnCount; ++column) {
            QVariant cellData = db_answer->data(db_answer->index(row, column));
            QStandardItem *item = new QStandardItem();
            item->setData(cellData, Qt::DisplayRole);
            rowItems.append(item);
        }
        model->appendRow(rowItems);
    }
    //Зададим заголовки таблиц
    if (ui->rb_arrival->isChecked() == true){
        model->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
        model->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
        model->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
    }
    else {
        model->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
        model->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
        model->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
    }

    //Т.к. по ТЗ требуется отображать только время, поэтому уберем дату из первого столбца
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex cellIndex = model->index(row, 1);
        //qDebug() << "Cell Index: " << cellIndex;
        QVariant cellData = model->data(cellIndex);
        //qDebug() << "Cell data: " << cellData;
        QTime cellTime = cellData.toTime();
        //qDebug() << "Cell time: " << cellData.toTime();
        model->setData(cellIndex, cellTime);
        //qDebug() << model->data(cellIndex);
    }

    //Сделаем выравнивание по центру
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QModelIndex index = model->index(row, column);
            QStandardItem *item = model->itemFromIndex(index);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    ui->tv_AirPortsTable->setModel(model);
    ui->tv_AirPortsTable->setShowGrid(true);
    ui->tv_AirPortsTable->resizeColumnsToContents();
}

