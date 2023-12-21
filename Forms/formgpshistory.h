#ifndef FORMGPSHISTORY_H
#define FORMGPSHISTORY_H

#include "../DataSource/EgDeviceData.h"
#include <QWidget>

class MapWidget;

namespace Ui
{
    class FormGPSHistory;
}

class FormGPSHistory : public QWidget
{
    Q_OBJECT

  public:
    explicit FormGPSHistory(QWidget* parent = nullptr);
    ~FormGPSHistory();

  public slots:
    void onGpsDataReady(EgGpsListData& gpsList);

  signals:
    void requestGpsHistoryData(int id, QDate& date);

  private slots:
    void on_listView_vehicles_clicked(const QModelIndex& index);

    void on_dateEdit_userDateChanged(const QDate& date);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

  private:
    Ui::FormGPSHistory* ui;
    void repaintGpsData();
    MapWidget* m_mapHistoryWidget;
    EgGpsListData m_gpsList;
};

#endif   // FORMGPSHISTORY_H
