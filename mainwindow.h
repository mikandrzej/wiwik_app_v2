#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include "QCustomPlot/qcustomplot.h"
#include "dialogassignsensor.h"
#include "dialogeditvehicles.h"
#include "egcarsmodel.h"
#include "egtemperaturechart.h"
#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  //  QCustomPlot *historyPlot() const;
  EgTemperatureChart *historyPlot();

  void setVehModel(EgCarsModel *newVehModel);

  DialogAssignSensor *dialogAssignSensor() const;
  QPushButton *pbAssignSensor() const;

  DialogEditVehicles *dialogEditVehicles() const;

  QPushButton *pbEditVehicles() const;
public slots:
  void setMqttStatus(bool status);
  void setRestStatus(bool status);

  void onHistoryDataReady(QVector<int> &timestamps,
                          QVector<double> &temperatures);
  void onSensorLiveDataReceived(EgSensorData &sensorData);
private slots:
  void onVehiclesModelDataChanged(const QModelIndex &topLeft,
                                  const QModelIndex &bottomRight,
                                  const QVector<int> &roles);
  void onLiveVehicleListClicked(const QModelIndex &index);

  void onPbHistoryTodayClicked(bool state);
  void onHistoryVehicleListClicked(const QModelIndex &index);
  void plotHistoryData();

  void on_pb_editVehicles_clicked();

  void onDeHistoryDateChanged(const QDate &date);
  void on_pb_historyReset_clicked();

  void on_pb_historyDateMinus_clicked();

  void on_pb_historyDatePlus_clicked();

  void on_pb_historySelectDate_clicked();

signals:
  void vehicleHistoryDataRequested(int, QDate &);

private:
  Ui::MainWindow *ui;
  QLabel *m_statusBarMqttLabel;
  QLabel *m_statusBarRestLabel;
  //  QCustomPlot *m_historyPlot;
  EgTemperatureChart *m_historyPlot;
  int m_liveVehicleListSelectedRow = -1;
  QModelIndex m_historyVehicleListSelectedIndex;

  EgCarsModel *m_vehModel;
  DialogAssignSensor *m_dialogAssignSensor;
  DialogEditVehicles *m_dialogEditVehicles;
  void historyDataAutoRescale();
};
#endif // MAINWINDOW_H
