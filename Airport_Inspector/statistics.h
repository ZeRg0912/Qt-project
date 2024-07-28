#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QCloseEvent>
#include "database.h"
#include "qcustomplot.h"

namespace Ui {
class Statistics;
}

class Statistics : public QWidget
{
    Q_OBJECT

public:

    explicit Statistics(QWidget *parent = nullptr);
    ~Statistics();

    /*!
     *  @brief Установить текст аэропорта.
     *  @param airport Название аэропорта и его код.
     */
    void setAirportText(const QString& airport);

    /*!
     *  @brief Установить данные за год по месяцам.
     *  @param data Вектор данных.
     */
    void setDataPerYear(const QVector<double> data);

    /*!
     *  @brief Установить данные за год по дням.
     *  @param data Мультикарта данных.
     */
    void setDataPerMonth(const QMultiMap<int, double> data);

    /*!
     *  @brief Установить статистику за год по дням.
     *  @details Используется при смене месяца в чекбоксе.
     *  @param month Месяц.
     */
    void setStatisticsPerMonth(const int month);

    /*!
     *  @brief Закрытие окна статистики.
     *  @param event Событие
     */
    void closeEvent(QCloseEvent* event) override;

    /*!
     *  @brief Определить месяц.
     *  @details Определяет номер месяца по тексту месяца.
     *  @param month Название месяца.
     */
    int findMonth(const QString& month);

signals:

    /*!
     *  @brief Сигнал: "Закрыть окно отображения статистики".
     */
    void sig_CloseStatistics();

private slots:

    /*!
     *  @brief Отработка нажатия на кнопку закрытия.
     */
    void on_pb_close_clicked();

    /*!
     *  @brief Отработка смены текста в чекбоксе.
     *  @param arg1 Текст в чекбоксе.
     */
    void on_cb_months_currentTextChanged(const QString &arg1);

private:
    Ui::Statistics *ui; //<! Главное окно статистики загруженности аэропорта.
    QMap<int, QString> months_; //<! Месяцы.
    QCPBars *CPBarsForYear_; //<! Гистограмма статистики за год.
    QMultiMap<int, double> dataMonth_; //<! Данные за месяцы.
    bool isDataMonthReady_; //<! Готовность данных о статистике за месяц.
};

#endif // STATISTICS_H
