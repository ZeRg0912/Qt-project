#include "StatisticWindow.h"
#include "ui_statisticwindow.h"

StatisticWindow::StatisticWindow(QWidget *parent) : QWidget(parent), ui(new Ui::StatisticWindow) {
    ui->setupUi(this);
    is_data_ready = false;

    ui->tabWidget->setCurrentWidget(ui->tab_perYear);
    months = { {1, "Январь"}, {2, "Февраль"}, {3, "Март"},
                {4, "Апрель"}, {5, "Май"}, {6, "Июнь"},
                {7, "Июль"}, {8, "Август"}, {9, "Сентябрь"},
                {10, "Октябрь"}, {11, "Ноябрь"}, {12, "Декабрь"}
    };

    for(const auto& [key, value] : months.asKeyValueRange()) {
        ui->cb_months->addItem(value);
    }

    CPBars_year = new QCPBars(  ui->widget_loadPerYear->xAxis,
                                ui->widget_loadPerYear->yAxis
    );
}

StatisticWindow::~StatisticWindow() {
    delete CPBars_year;
    delete ui;
}

void StatisticWindow::on_cb_months_currentTextChanged(const QString &arg1) {
    if (is_data_ready) {
        SetStatisticsPerMonth(FindMonth(arg1));
    }
}

void StatisticWindow::SetAirportText(const QString &airport) {
    ui->lb_airportsLoad->setText("Загруженность аэропорта " + airport);
}

void StatisticWindow::SetDataPerYear(const QVector<double> data) {
    ui->widget_loadPerYear->clearGraphs();

    QVector<double> ticks;
    QVector<QString> labels;

    for(const auto& [key, value] : months.asKeyValueRange()) {
        ticks << key;
        labels << value;
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->widget_loadPerYear->xAxis->setTicker(textTicker);
    ui->widget_loadPerYear->xAxis->setTickLabelRotation(60);
    ui->widget_loadPerYear->xAxis->setSubTicks(false);
    ui->widget_loadPerYear->xAxis->setTickLength(0, 4);
    ui->widget_loadPerYear->xAxis->setLabel("Месяцы");
    ui->widget_loadPerYear->xAxis->setRange(0, months.size() + 1);
    ui->widget_loadPerYear->xAxis->grid()->setVisible(true);

    ui->widget_loadPerYear->yAxis->setRange(0, *std::max_element(data.begin(), data.end()) + 1);
    ui->widget_loadPerYear->yAxis->setPadding(5);
    ui->widget_loadPerYear->yAxis->setLabel("Загруженность аэропорта за год");
    ui->widget_loadPerYear->yAxis->grid()->setSubGridVisible(true);
    ui->widget_loadPerYear->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->widget_loadPerYear->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    CPBars_year->setData(ticks, data);
    ui->widget_loadPerYear->replot();
}

void StatisticWindow::SetDataPerMonth(const QMultiMap<int, double> data) {
    month_data = data;
    is_data_ready = !is_data_ready;
    SetStatisticsPerMonth(1);
}

void StatisticWindow::SetStatisticsPerMonth(const int month) {
    ui->widget_loadPerMonth->clearGraphs();
    ui->widget_loadPerMonth->addGraph();
    ui->widget_loadPerMonth->graph(0)->setPen(QPen(Qt::blue));
    QVector<double> x, y;
    int it = 0;

    for (const auto& [key, value] : month_data.asKeyValueRange()) {
        if (key == month) {
            ++it;
            y.append(value);
            x.append(it);
        }
    }

    ui->widget_loadPerMonth->graph(0)->setData(x, y);
    ui->widget_loadPerMonth->graph(0)->rescaleAxes();

    ui->widget_loadPerMonth->replot();
}

int StatisticWindow::FindMonth(const QString &arg1) {
    for (const auto& [key, value] : months.asKeyValueRange()) {
        if (arg1 == value) {
            return key;
        }
    }
    return -1;
}

void StatisticWindow::on_pb_close_clicked() {
    is_data_ready = !is_data_ready;
    ui->cb_months->setCurrentText(months[1]);
    emit sig_CloseStatistics();
}

void StatisticWindow::closeEvent(QCloseEvent *event) {
    is_data_ready = !is_data_ready;
    ui->cb_months->setCurrentText(months[1]);
    emit sig_CloseStatistics();
}

