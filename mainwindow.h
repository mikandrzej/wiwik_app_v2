#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include "QCustomPlot/qcustomplot.h"
#include <QLabel>
#include <QMainWindow>
#include "Forms/formgpshistory.h"
#include "chartwidget.h"
#include "dialogassignsensor.h"
#include "dialogeditvehicles.h"
#include "egvehiclesmap.h"
#include "historyvehiclesproxymodel.h"
#include "livevehiclesproxymodel.h"
#include "liveviewmodel.h"
#include "mapwidget.h"
#include "vehiclesmodel.h"

QT_BEGIN_NAMESPACE

class MainVehicleModel;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void setVehModel(VehiclesModel *newVehModel);
  void setLiveViewModel(LiveViewModel *newLiveViewModel);

  DialogAssignSensor *dialogAssignSensor() const;
  QPushButton *pbAssignSensor() const;

  DialogEditVehicles *dialogEditVehicles() const;

  QPushButton *pbEditVehicles() const;
  ChartWidget *historyPlot();
  void setMainVehicleModel(MainVehicleModel *newMainVehicleModel);

  FormGPSHistory *formGpsHistory() const;

  public slots:
  void setMqttStatus(bool status);
  void setRestStatus(bool status);

  void onSensorLiveDataReceived(EgVehicleSensorData &sensorData);
  void onMapMarkerClicked(int vehicleId);
  void onHistoryDataReady(EgTemperatureListData &tempListData);
private slots:
  void onVehiclesModelDataChanged(const QModelIndex &topLeft,
                                  const QModelIndex &bottomRight,
                                  const QVector<int> &roles);
  void onLiveVehicleListClicked(const QModelIndex &index);

  void onPbHistoryTodayClicked(bool state);
  void onTemperatureHistoryVehicleListClicked(const QModelIndex &index);
  void plotHistoryData();

  void on_pb_editVehicles_clicked();

  void onDeHistoryDateChanged(const QDate &date);
  void on_pb_historyReset_clicked();

  void on_pb_historyDateMinus_clicked();

  void on_pb_historyDatePlus_clicked();

  void on_pb_historySelectDate_clicked();

  void onGpsHistoryVehicleListClicked(const QModelIndex &index);
  void onHistoryMapMarkerClicked(int vehicleId);

  signals:
  void vehicleHistoryDataRequested(int, QDate &);

private:
  Ui::MainWindow *ui;
  FormGPSHistory *m_formGpsHistory = nullptr;

  QLabel *m_statusBarMqttLabel;
  QLabel *m_statusBarRestLabel;
  ChartWidget *m_historyPlot;
  int m_liveVehicleListSelectedRow = -1;
  QModelIndex m_historyVehicleListSelectedIndex;

  VehiclesModel *m_vehModel;
  LiveVehiclesProxyModel *m_liveVehModel;
  HistoryVehiclesProxyModel *m_historyVehModel;
  EgVehiclesMap *m_mapWidget;
  MapWidget *m_mapHistoryWidget;
  DialogAssignSensor *m_dialogAssignSensor;
  DialogEditVehicles *m_dialogEditVehicles;
  void historyDataAutoRescale();
  QModelIndex getVehicleModelIndexById(const int vehicleId);
  void updateLiveEditFields(const QModelIndex &index);

  // data models
  MainVehicleModel *m_mainVehicleModel;
};
#endif // MAINWINDOW_H
