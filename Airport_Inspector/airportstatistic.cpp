#include "airportstatistic.h"
#include "ui_airportstatistic.h"

AirportStatistic::AirportStatistic(QWidget *parent, QString name_) : QWidget(parent), name(name_), ui(new Ui::AirportStatistic) {
    ui->setupUi(this);
    InitialSetup();
    ChartsSetup();

    QVector<QPointF> year_data;
    QVector<QPointF> month_data;
    for (size_t i = 0; i < 12; i++) {
        year_data.append(QPointF(i + 1, i));
    }
    for (size_t i = 0; i < 32; i++) {
        month_data.append(QPointF(i + 1, i));
    }

    UpdateGraph(year_data, year_chart_view);
    UpdateGraph(month_data, month_chart_view);
}

AirportStatistic::~AirportStatistic() {
    delete ui;
}

void AirportStatistic::InitialSetup(){
    QVector<QString> month_names = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
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

    //!< Установка осей для Годового графика
    QValueAxis* year_AxisX = new QValueAxis(this);
    year_AxisX->setTitleText("Месяцы");
    font = year_AxisX->titleFont();
    font.setBold(true);
    year_AxisX->setTitleFont(font);
    year_AxisX->setLabelFormat("%i");
    year_chart->addAxis(year_AxisX, Qt::AlignBottom);
    year_series->attachAxis(year_AxisX);

    QValueAxis* year_AxisY = new QValueAxis(this);
    year_AxisY->setTitleText("Кол-во рейсов");
    year_AxisY->setLabelFormat("%g");
    year_chart->addAxis(year_AxisY, Qt::AlignLeft);
    year_series->attachAxis(year_AxisY);

    //!< Установка осей для Месячного графика
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
    month_AxisY->setLabelFormat("%g");
    month_chart->addAxis(month_AxisY, Qt::AlignLeft);
    month_series->attachAxis(month_AxisY);

    //!< Настройка графиков
    year_chart_view->setRenderHint(QPainter::Antialiasing);
    month_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->wid_YearChart->setLayout(new QVBoxLayout());
    ui->wid_YearChart->layout()->addWidget(year_chart_view);

    ui->wid_MonthChart->setLayout(new QVBoxLayout());
    ui->wid_MonthChart->layout()->addWidget(month_chart_view);
}

void AirportStatistic::UpdateGraph(const QVector<QPointF>& data, QChartView* chart_view) {
    if (!chart_view) return;

    QChart* chart = chart_view->chart();
    if(!chart) return;

    QList<QAbstractSeries*> series_list = chart->series();
    if (series_list.isEmpty()) return;

    QLineSeries* series = qobject_cast<QLineSeries*>(series_list.first());
    if (!series) return;
    series->clear();
    series->append(data);

    QList<QAbstractAxis*> axesX = chart->axes(Qt::Horizontal);
    QList<QAbstractAxis*> axesY = chart->axes(Qt::Vertical);

    if (axesX.isEmpty() || axesY.isEmpty()) return;

    QValueAxis* axisX = static_cast<QValueAxis*>(axesX.first());
    QValueAxis* axisY = static_cast<QValueAxis*>(axesY.first());

    qreal minX = data.first().x();
    qreal maxX = data.last().x();
    qreal minY = data.first().y();
    qreal maxY = minY;

    for (const auto& point : data) {
        if (point.x() < minX) minX = point.x();
        if (point.x() > maxX) maxX = point.x();
        if (point.x() < minY) minY = point.y();
        if (point.x() > maxY) maxY = point.y();
    }

    axisX->setRange(minX, maxX);
    axisY->setRange(minY, maxY);
}

void AirportStatistic::on_pb_Close_clicked() {
    this->close();
}

void AirportStatistic::on_cb_Months_highlighted(int index) {
    if (index >= 0) {
        if (month_chart_view != nullptr) {
            QVector<QPointF> month_data;
            for (size_t i = 0; i <= index + 1; i++) {
                month_data.append(QPointF(i, i));
            }

            UpdateGraph(month_data, month_chart_view);
        }
    }
}

