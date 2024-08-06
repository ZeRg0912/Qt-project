#include "airport_statistic.h"
#include "ui_airport_statistic.h"
/*!
 * @brief Конструктор
 */
AirportStatistic::AirportStatistic(QWidget *parent)
    : QWidget(parent), ui(new Ui::AirportStatistic) {
    ui->setupUi(this);
    InitialSetup();
    ChartsSetup();

    year_data = new QVector<QPointF>();
    months_data = new QMap<int, QVector<QPointF>>();
}
/*!
 * @brief Деструктор
 */
AirportStatistic::~AirportStatistic() {
    delete year_data;
    delete months_data;
    delete ui;
}
/*!
 * @brief Установить имя выбранного аэропорта
 * @param name_ имя
 */
void AirportStatistic::SetAirportName(QString name_) {
    this->name = name_;
    ui->lb_Airport->setText("Загруженность аэропорта " + name);
}
/*!
 * @brief Первоначальная настройка в конструкторе
 */
void AirportStatistic::InitialSetup() {
    ui->tabWidget->setCurrentIndex(0);
    ui->cb_Months->setCurrentIndex(0);    

    ui->pb_Close->setText("Закрыть статистику");
    ui->tabWidget->setTabText(0, "За год");
    ui->tabWidget->setTabText(1, "За месяц");
}
/*!
 * @brief Первоначальная настройка графиков
 */
void AirportStatistic::ChartsSetup() {
    year_chart = new QChart();
    year_chart_view = new QChartView(year_chart);
    year_series = new QBarSeries(this);

    month_chart = new QChart();
    month_chart_view = new QChartView(month_chart);
    month_series = new QLineSeries(this);

    for (size_t i = 0; i < month_names.size(); i++) {
        months.insert(i + 1, month_names[i]);
    }
    for (auto it = months.begin(); it != months.end(); it++) {
        ui->cb_Months->addItem(it.value());
    }

    year_chart->addSeries(year_series);
    year_chart->legend()->hide();
    year_chart->setTitle("Статистика за год:");
    year_chart->setAnimationOptions(QChart::SeriesAnimations);
    QFont font = year_chart->titleFont();
    font.setBold(true);
    year_chart->setTitleFont(font);

    month_chart->addSeries(month_series);
    month_chart->legend()->hide();
    month_chart->setTitle("Статистика за месяц:");
    month_chart->setAnimationOptions(QChart::SeriesAnimations);
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
    year_AxisX->setRange(-1, 12);
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
/*!
 * @brief Получение данных за год
 */
void AirportStatistic::rcv_DataPerYear(QSqlQueryModel* model) {
    year_data->clear();
    //qDebug() << "Получены данные за год. Количество строк:" << model->rowCount();
    for (int i = 0; i < model->rowCount(); i++) {
        int flights_count = model->data(model->index(i, 0)).toInt();
        QDate month_date = model->data(model->index(i, 1)).toDate();
        int month_number = month_date.month();
        year_data->append(QPointF(month_number - 1, flights_count));
        //qDebug() << "Месяц:" << month_number << "Рейсы:" << flights_count;
    }

    std::sort(year_data->begin(), year_data->end(), [](const QPointF& a, const QPointF& b) {
            return a.x() < b.x();
        });

    UpdateYearGraph();
}
/*!
 * @brief Получение данных по месяцам
 */
void AirportStatistic::rcv_DataPerMonth(QSqlQueryModel* model) {
    months_data->clear();

        //qDebug() << "Получены данные за месяц. Количество строк:" << model->rowCount();

        for (int i = 0; i < model->rowCount(); i++) {
            int flights_count = model->data(model->index(i, 0)).toInt();
            QDate day_date = model->data(model->index(i, 1)).toDate();
            int day_number = day_date.day();
            int month_number = day_date.month();

            if (!months_data->contains(month_number)) {
                months_data->insert(month_number, QVector<QPointF>());
            }

            QVector<QPointF>& data = months_data->operator[](month_number);
            data.append(QPointF(day_number, flights_count));
            //qDebug() << "Месяц:" << month_number << "День:" << day_number << "Рейсы:" << flights_count;
        }

        int current_month = ui->cb_Months->currentIndex() + 1;
        UpdateMonthGraph(current_month);
}
/*!
 * @brief Обновленя графика за год
 */
void AirportStatistic::UpdateYearGraph() {
    year_series->clear();
    QBarSet *set = new QBarSet("Flights");

    for (const QPointF &point : *year_data) {
        *set << point.y();
    }

    year_series->append(set);
    //qDebug() << "Обновление графика за год с данными:" << *year_data;

    QList<QAbstractAxis*> axes = year_chart->axes(Qt::Vertical);
    if (!axes.isEmpty()) {
        QValueAxis* axisY = static_cast<QValueAxis*>(axes.first());
        float minY = 0;
        float maxY = 0;

        for (const QPointF& point : *year_data) {
            if (point.y() < minY) {
                minY = point.y();
            }
            if (point.y() > maxY) {
                maxY = point.y();
            }
        }

        axisY->setRange(minY, maxY + 10);
    }
}
/*!
 * @brief Обновление графика за выбранный месяц
 * @param month_index выбранный месяц
 */
void AirportStatistic::UpdateMonthGraph(int month_index) {
    month_series->clear();

    if (!months_data->contains(month_index)) {
        //qDebug() << "Нет данных для отображения.";
        return;
    }

    const QVector<QPointF>& data = months_data->operator[](month_index);
    month_series->append(data);
    //qDebug() << "Обновление графика за месяц для месяца:" << month_index << "с данными:" << data;

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
        axisY->setRange(minY - 1, maxY + 1);

        axisY->setTickCount((maxY - minY) + 3);
        //axisY->setMinorTickCount((maxY - minY) + 1);
        //qDebug() << "Диапазон X-оси графика за месяц установлен на:" << minX << "до" << maxX;
        //qDebug() << "Диапазон Y-оси графика за месяц установлен на:" << minY << "до" << maxY;

        month_chart->update();
    }
}
/*!
 * @brief Вспомогательная функция для проверки записанных данных
 */
void AirportStatistic::PrintStoredData() {
    //qDebug() << "Данные за год:";
    for (const QPointF& point : *year_data) {
        //qDebug() << "Месяц:" << point.x() << "Рейсы:" << point.y();
    }

    //qDebug() << "Данные по месяцам:";
    for (auto it = months_data->constBegin(); it != months_data->constEnd(); it++) {
        //qDebug() << "Месяц:" << it.key();
        for (const QPointF& point : it.value()) {
            //qDebug() << "  День:" << point.x() << "Рейсы:" << point.y();
        }
    }
}
/*!
 * @brief Закрытие окна
 */
void AirportStatistic::on_pb_Close_clicked() {
    ui->cb_Months->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
    emit sig_Closed();
    close();
}
/*!
 * @brief Смена месяца
 * @param index индекс месяца
 */
void AirportStatistic::on_cb_Months_highlighted(int index) {
    int month = index + 1;
    UpdateMonthGraph(month);
    //PrintStoredData();
}

void AirportStatistic::closeEvent(QCloseEvent *event) {
    ui->cb_Months->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
    emit sig_Closed();
    QWidget::closeEvent(event);
}
