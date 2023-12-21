#ifndef APP_H
#define APP_H

#include "DataModels/mainvehiclemodel.h"
#include "DataSource/egdataprovider.h"
#include "egcarsmapmodel.h"
#include "egsettings.h"
#include "mainwindow.h"
#include "vehicleseditmodel.h"
#include "vehiclesmodel.h"
#include <QObject>

class App : public QObject {
  Q_OBJECT
public:
  explicit App(MainWindow *mainWindow, QObject *parent = nullptr);

signals:
  void historyDataReady(EgTemperatureListData &tempListData);
private slots:
  void onSensorDataReceived(EgSensorData &sensorData);
  void onRestServerStateChanged(bool state);

  void onAssignSensorClicked(bool checked);

  void onEditVehiclesClicked(bool checked);

private:
  EgSettings m_egSettings;
  VehiclesModel m_vehiclesModel;
  VehiclesEditModel m_vehiclesEditModel;
  EgCarsMapModel m_carsMapModel;
  MainWindow *m_mainWindow;
  EgDataProvider m_dataProvider;
  void bindGui();

  // data models
  MainVehicleModel m_mainVehicleModel;
};

#endif // APP_H
