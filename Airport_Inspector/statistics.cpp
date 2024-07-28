#include "statistics.h"
#include "ui_statistics.h"

Statistics::Statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);
    isDataMonthReady_ = false;
    ui->tabWidget->setCurrentWidget(ui->tab_perYear);
    months_ = { {1, "Январь"}, {2, "Февраль"}, {3, "Март"},
                {4, "Апрель"}, {5, "Май"}, {6, "Июнь"},
                {7, "Июль"}, {8, "Август"}, {9, "Сентябрь"},
                {10, "Октябрь"}, {11, "Ноябрь"}, {12, "Декабрь"}
              };
//    ui>setWindowTitle("Статистика");

    for(const auto& [key, value] : months_.asKeyValueRange())
    {
        ui->cb_months->addItem(value);
    }

    CPBarsForYear_ = new QCPBars(ui->widget_loadPerYear->xAxis,
                                 ui->widget_loadPerYear->yAxis
                                 );


}

Statistics::~Statistics()
{
    delete CPBarsForYear_;
    delete ui;
}

void Statistics::on_cb_months_currentTextChanged(const QString &arg1)
{
    if (isDataMonthReady_)
    {
        setStatisticsPerMonth(findMonth(arg1));
    }
}

void Statistics::setAirportText(const QString &airport)
{
    ui->l_airportsLoad->setText("Загруженность аэропорта " + airport);
}

void Statistics::setDataPerYear(const QVector<double> data)
{
    ui->widget_loadPerYear->clearGraphs();

    QVector<double> ticks;
    QVector<QString> labels;
    for(const auto& [key, value] : months_.asKeyValueRange())
    {
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
    ui->widget_loadPerYear->xAxis->setRange(0, months_.size() + 1);
    ui->widget_loadPerYear->xAxis->grid()->setVisible(true);

    ui->widget_loadPerYear->yAxis->setRange(0, *std::max_element(data.begin(), data.end()) + 1);
    ui->widget_loadPerYear->yAxis->setPadding(5);
    ui->widget_loadPerYear->yAxis->setLabel("Загруженность аэропорта за год");
    ui->widget_loadPerYear->yAxis->grid()->setSubGridVisible(true);
    ui->widget_loadPerYear->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->widget_loadPerYear->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    CPBarsForYear_->setData(ticks, data);
    ui->widget_loadPerYear->replot();
}

void Statistics::setDataPerMonth(const QMultiMap<int, double> data)
{
    dataMonth_ = data;
    isDataMonthReady_ = !isDataMonthReady_;
    setStatisticsPerMonth(1);
}

void Statistics::setStatisticsPerMonth(const int month)
{

    ui->widget_loadPerMonth->clearGraphs();
    ui->widget_loadPerMonth->addGraph();
    ui->widget_loadPerMonth->graph(0)->setPen(QPen(Qt::blue));
    QVector<double> x, y;
    int it = 0;
    for (const auto& [key, value] : dataMonth_.asKeyValueRange())
    {
        if (key == month)
        {
            ++it;
            y.append(value);
            x.append(it);
        }
    }
    ui->widget_loadPerMonth->graph(0)->setData(x, y);
    ui->widget_loadPerMonth->graph(0)->rescaleAxes();

    ui->widget_loadPerMonth->replot();
}

int Statistics::findMonth(const QString &arg1)
{
    for (const auto& [key, value] : months_.asKeyValueRange())
    {
        if (arg1 == value)
        {
            return key;
        }
    }
    return -1;
}

void Statistics::on_pb_close_clicked()
{
    isDataMonthReady_ = !isDataMonthReady_;
    ui->cb_months->setCurrentText(months_[1]);
    emit sig_CloseStatistics();
}

void Statistics::closeEvent(QCloseEvent *event)
{
    isDataMonthReady_ = !isDataMonthReady_;
    ui->cb_months->setCurrentText(months_[1]);
    emit sig_CloseStatistics();
}

