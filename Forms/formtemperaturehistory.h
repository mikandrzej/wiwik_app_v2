#ifndef FORMTEMPERATUREHISTORY_H
#define FORMTEMPERATUREHISTORY_H

#include <QModelIndex>
#include <QWidget>

class ChartWidget;
struct EgTemperatureListData;
namespace Ui
{
    class FormTemperatureHistory;
}

class FormTemperatureHistory : public QWidget
{
    Q_OBJECT

  public:
    explicit FormTemperatureHistory(QWidget* parent = nullptr);
    ~FormTemperatureHistory() override;

  public slots:
    void onTemperatureDataReady(EgTemperatureListData& temperatureData);
  signals:
    void requestTemperatureHistoryData(int id, QDate& date);

  private slots:
    void on_listView_vehicles_clicked(const QModelIndex& index);

    void on_pb_historyToday_clicked();

  private:
    Ui::FormTemperatureHistory* ui;
    ChartWidget* m_chartWidget;
    QModelIndex m_selectedIndex;
    void refreshTemperatureData();
};

#endif   // FORMTEMPERATUREHISTORY_H
