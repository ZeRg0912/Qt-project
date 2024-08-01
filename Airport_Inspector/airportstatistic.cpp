#include "airportstatistic.h"
#include "ui_airportstatistic.h"

AirportStatistic::AirportStatistic(QWidget *parent, QString name_)
    : QWidget(parent), name(name_), ui(new Ui::AirportStatistic) {
    ui->setupUi(this);
    InitialSetup();
    ChartsSetup();

    year_data = new QVector<QPointF>();
    months_data = new QMap<int, QVector<QPointF>>();
}

AirportStatistic::~AirportStatistic() {
    delete year_data;
    delete months_data;
    delete ui;
}

void AirportStatistic::InitialSetup() {
    for (size_t i = 0; i < month_names.size(); i++) {
        months.insert(i + 1, month_names[i]);
    }
    for (auto it = months.begin(); it != months.end(); it++) {
        ui->cb_Months->addItem(it.value());
    }

    ui->lb_Airport->setText("Загруженность аэропорта " + name);
    ui->pb_Close->setText("Закрыть статистику");
    ui->tabWidget->setTabText(0, "За год");
    ui->tabWidget->setTabText(1, "За месяц");
}

void AirportStatistic::ChartsSetup() {
    year_chart = new QChart();
    year_chart_view = new QChartView(year_chart);
    year_series = new QLineSeries(this);

    month_chart = new QChart();
    month_chart_view = new QChartView(month_chart);
    month_series = new QLineSeries(this);

    year_chart->addSeries(year_series);
    year_chart->legend()->hide();
    year_chart->setTitle("Статистика за год:");
    QFont font = year_chart->titleFont();
    font.setBold(true);
    year_chart->setTitleFont(font);

    month_chart->addSeries(month_series);
    month_chart->legend()->hide();
    month_chart->setTitle("Статистика за месяц:");
    font = month_chart->titleFont();
    font.setBold(true);
    month_chart->setTitleFont(font);

    QCategoryAxis* year_AxisX = new QCategoryAxis(this);
    year_AxisX->setTitleText("Месяцы");
    font = year_AxisX->titleFont();
    font.setBold(true);
    year_AxisX->setTitleFont(font);
    for (size_t i = 0; i < month_names.size(); i++) {
        year_AxisX->append(month_names[i], i + 1);
    }
    year_AxisX->setLabelFormat("%s");
    year_AxisX->setLabelsAngle(45);
    year_AxisX->setRange(0, 12);
    year_chart->addAxis(year_AxisX, Qt::AlignBottom);
    year_series->attachAxis(year_AxisX);

    QValueAxis* year_AxisY = new QValueAxis(this);
    year_AxisY->setTitleText("Кол-во рейсов");
    year_AxisY->setLabelFormat("%i");
    year_chart->addAxis(year_AxisY, Qt::AlignLeft);
    year_series->attachAxis(year_AxisY);

    QValueAxis* month_AxisX = new QValueAxis(this);
    month_AxisX->setTitleText("дни");
    font = month_AxisX->titleFont();
    font.setBold(true);
    month_AxisX->setTitleFont(font);
    month_AxisX->setLabelFormat("%i");
    month_chart->addAxis(month_AxisX, Qt::AlignBottom);
    month_series->attachAxis(month_AxisX);

    QValueAxis* month_AxisY = new QValueAxis(this);
    month_AxisY->setTitleText("Кол-во рейсов");
    month_AxisY->setLabelFormat("%i");
    month_chart->addAxis(month_AxisY, Qt::AlignLeft);
    month_series->attachAxis(month_AxisY);

    year_chart_view->setRenderHint(QPainter::Antialiasing);
    month_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->wid_YearChart->setLayout(new QVBoxLayout());
    ui->wid_YearChart->layout()->addWidget(year_chart_view);

    ui->wid_MonthChart->setLayout(new QVBoxLayout());
    ui->wid_MonthChart->layout()->addWidget(month_chart_view);
}

void AirportStatistic::rcv_DataPerYear(QSqlQueryModel* model) {
    year_data->clear();
    qDebug() << "Получены данные за год. Количество строк:" << model->rowCount();
    for (int i = 0; i < model->rowCount(); ++i) {
        int flights_count = model->data(model->index(i, 0)).toInt();
        QDate month_date = model->data(model->index(i, 1)).toDate();
        int month_number = month_date.month();
        year_data->append(QPointF(month_number, flights_count));
        qDebug() << "Месяц:" << month_number << "Рейсы:" << flights_count;
    }
    UpdateYearGraph();
}

void AirportStatistic::rcv_DataPerMonth(QSqlQueryModel* model) {
    int current_month = ui->cb_Months->currentIndex() + 1; // +1 для соответствия с индексами месяцев

    if (!months_data->contains(current_month)) {
        months_data->insert(current_month, QVector<QPointF>());
    }

    QVector<QPointF>& data = months_data->operator[](current_month);
    data.clear(); // Очистка данных перед добавлением новых

    qDebug() << "Получены данные за месяц для месяца:" << current_month << "Количество строк:" << model->rowCount();

    for (int i = 0; i < model->rowCount(); ++i) {
        int flights_count = model->data(model->index(i, 0)).toInt();
        QDate day_date = model->data(model->index(i, 1)).toDate();
        int day_number = day_date.day();
        data.append(QPointF(day_number, flights_count));
        qDebug() << "День:" << day_number << "Рейсы:" << flights_count;
    }

    UpdateMonthGraph(current_month);
    PrintStoredData(); // Проверка сразу после обновления
}

void AirportStatistic::UpdateYearGraph() {
    year_series->clear();
    year_series->append(*year_data);
    qDebug() << "Обновление графика за год с данными:" << *year_data;

    QList<QAbstractAxis*> axes = year_chart->axes(Qt::Vertical);
    if (!axes.isEmpty()) {
        QValueAxis* axisY = static_cast<QValueAxis*>(axes.first());

        qreal minY = year_data->isEmpty() ? 0 : year_data->first().y();
        qreal maxY = year_data->isEmpty() ? 0 : year_data->last().y();

        for (const auto& point : *year_data) {
            if (point.y() < minY) minY = point.y();
            if (point.y() > maxY) maxY = point.y();
        }

        axisY->setRange(minY, maxY);
        qDebug() << "Диапазон Y-оси графика за год установлен на:" << minY << "до" << maxY;
    }
}

void AirportStatistic::UpdateMonthGraph(int month_index) {
    month_series->clear();

    if (!months_data->contains(month_index)) {
        qDebug() << "Нет данных для отображения.";
        return;
    }

    const QVector<QPointF>& data = months_data->operator[](month_index);
    month_series->append(data);
    qDebug() << "Обновление графика за месяц для месяца:" << month_index << "с данными:" << data;

    QList<QAbstractAxis*> axesX = month_chart->axes(Qt::Horizontal);
    QList<QAbstractAxis*> axesY = month_chart->axes(Qt::Vertical);

    if (!axesX.isEmpty() && !axesY.isEmpty()) {
        QValueAxis* axisX = static_cast<QValueAxis*>(axesX.first());
        QValueAxis* axisY = static_cast<QValueAxis*>(axesY.first());

        qreal minX = data.first().x();
        qreal maxX = data.first().x();
        qreal minY = data.first().y();
        qreal maxY = data.first().y();

        for (const auto& point : data) {
            if (point.x() < minX) minX = point.x();
            if (point.x() > maxX) maxX = point.x();
            if (point.y() < minY) minY = point.y();
            if (point.y() > maxY) maxY = point.y();
        }

        axisX->setRange(minX, maxX);
        axisY->setRange(minY, maxY);

        qDebug() << "Диапазон X-оси графика за месяц установлен на:" << minX << "до" << maxX;
        qDebug() << "Диапазон Y-оси графика за месяц установлен на:" << minY << "до" << maxY;

        month_chart->update(); // Обновите график после изменения диапазонов
    }
}

void AirportStatistic::PrintStoredData() {
    // Проверка данных за год
    qDebug() << "Данные за год:";
    for (const QPointF& point : *year_data) {
        qDebug() << "Месяц:" << point.x() << "Рейсы:" << point.y();
    }

    // Проверка данных по месяцам
    qDebug() << "Данные по месяцам:";
    for (auto it = months_data->constBegin(); it != months_data->constEnd(); ++it) {
        qDebug() << "Месяц:" << it.key();
        for (const QPointF& point : it.value()) {
            qDebug() << "  День:" << point.x() << "Рейсы:" << point.y();
        }
    }
}

void AirportStatistic::on_pb_Close_clicked() {
    close();
}

void AirportStatistic::on_cb_Months_highlighted(int index) {
    int month = index + 1; // Месяцы начинаются с 1
    if (months_data->contains(month)) {
        UpdateMonthGraph(month);
    } else {
        qDebug() << "Нет данных для месяца:" << month;
        month_series->clear(); // Очистка графика, если данных нет
    }
    PrintStoredData();
}
