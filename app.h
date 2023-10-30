#ifndef APP_H
#define APP_H

#include "dialogassignsensor.h"
#include "egcarsmodel.h"
#include "egdataprovider.h"
#include "eghistoryvehiclesmodel.h"
#include "egsettings.h"
#include "mainwindow.h"
#include <QObject>

class App : public QObject {
  Q_OBJECT
public:
  explicit App(MainWindow *mainWindow, QObject *parent = nullptr);

signals:
  void historyDataReady(QVector<int> &timestamps,
                        QVector<double> &temperatures);
private slots:
  void onVehiclesDataReceived(EgVehicleListData &vehiclesData);
  void onSensorDataReceived(EgSensorData &sensorData);
  void onRestServerStateChanged(bool state);

  void onAssignSensorClicked(bool checked);

  void onEditVehiclesClicked(bool checked);
  private:
  EgSettings m_egSettings;
  EgCarsModel m_carsModel;
  EgHistoryVehiclesModel m_historyVehiclesModel;
  MainWindow *m_mainWindow;
  EgDataProvider m_dataProvider;
  void bindGui();
};

#endif // APP_H
