#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include "database.h"
#include "qcustomplot.h"

namespace Ui {
class StatisticWindow;
}

class StatisticWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticWindow(QWidget *parent = nullptr);
    ~StatisticWindow();

    /*!
     *  @brief Установить текст аэропорта.
     *  @param airport Название аэропорта и его код.
     */
    void SetAirportText(const QString& airport);

    /*!
     *  @brief Установить данные за год по месяцам.
     *  @param data Вектор данных.
     */
    void SetDataPerYear(const QVector<double> data);

    /*!
     *  @brief Установить данные за год по дням.
     *  @param data Мультикарта данных.
     */
    void SetDataPerMonth(const QMultiMap<int, double> data);

    /*!
     *  @brief Установить статистику за год по дням.
     *  @details Используется при смене месяца в чекбоксе.
     *  @param month Месяц.
     */
    void SetStatisticsPerMonth(const int month);

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
    int FindMonth(const QString& month);

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
    //!< Окно статистики загруженности аэропорта.
    Ui::StatisticWindow *ui;
    //!< Месяцы.
    QMap<int, QString> months;
    //!< Гистограмма статистики за год.
    QCPBars *CPBars_year;
    //!< Данные за месяцы.
    QMultiMap<int, double> month_data;
    //!< Готовность данных о статистике за месяц.
    bool is_data_ready;
};

#endif // STATISTICWINDOW_H
