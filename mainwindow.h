#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include "QCustomPlot/qcustomplot.h"
#include <QErrorMessage>
#include <QLabel>
#include <QMainWindow>
#include "Forms/formgpshistory.h"
#include "Forms/formliveview.h"
#include "Forms/formtemperaturehistory.h"
#include "chartwidget.h"
#include "dialogassignsensor.h"
#include "dialogeditvehicles.h"
#include "egvehiclesmap.h"
#include "historyvehiclesproxymodel.h"
#include "livevehiclesproxymodel.h"
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

  DialogAssignSensor *dialogAssignSensor() const;
  QPushButton *pbAssignSensor() const;

  DialogEditVehicles *dialogEditVehicles() const;

  QPushButton *pbEditVehicles() const;
  ChartWidget *historyPlot();

  FormGPSHistory *formGpsHistory() const;

  FormLiveView *formLiveView() const;

  FormTemperatureHistory *formTemperatureHistory() const;

  public slots:
  void setMqttStatus(bool status);
  void setRestStatus(bool status);

  private slots:

  void on_pb_editVehicles_clicked();

  void onGpsHistoryVehicleListClicked(const QModelIndex &index);

  signals:
  void vehicleHistoryDataRequested(int, QDate &);

  private:
  Ui::MainWindow *ui;
  FormGPSHistory *m_formGpsHistory = nullptr;
  FormLiveView *m_formLiveView = nullptr;
  FormTemperatureHistory *m_formTemperatureHistory = nullptr;

  QLabel *m_statusBarMqttLabel;
  QLabel *m_statusBarRestLabel;
  int m_liveVehicleListSelectedRow = -1;
  QModelIndex m_historyVehicleListSelectedIndex;

  DialogAssignSensor *m_dialogAssignSensor;
  DialogEditVehicles *m_dialogEditVehicles;
  void historyDataAutoRescale();
  QModelIndex getVehicleModelIndexById(const int vehicleId);

  // data models
  MainVehicleModel *m_mainVehicleModel;
};
#endif // MAINWINDOW_H
