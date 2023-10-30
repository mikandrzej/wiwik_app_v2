#ifndef EGDATAPROVIDER_H
#define EGDATAPROVIDER_H

#include "EgVehicleData.h"
#include "egmqttdatasource.h"
#include "egrestdatasource.h"

#include <QObject>

class EgDataProvider : public QObject {
  Q_OBJECT
public:
  explicit EgDataProvider(QObject *parent = nullptr);

  void requestVehiclesData();
  void requestDevicesData();

signals:
  void vehicleDataReceived(EgVehicleListData &vehiclesData);
  void devicesDataReceived(EgDevicesListData &devicesData);
  void sensorLiveDataReceived(EgSensorData &sensorData);
  void mqttServerStateChanged(bool state);
  void restServerStateChanged(bool state);
  void vehiclesHistoryDataReady(QVector<int> &, QVector<double> &);
  void onAddNewVehicle(QString &vehName, QString &plateNo);

public slots:
  void assignVehicleToDevice(EgDeviceData *selectedDevice,
                             EgVehicleData *selectedVehicle);
  void requestVehicleHistoryData(int vehicleId, QDate &date);

public:
  const EgDevicesListData &getDevicesList();
  const EgVehicleListData &getVehiclesList();

private:
  EgMqttDataSource *m_mqttDataSource;
  EgRestDataSource *m_restDataSource;
  EgDevicesListData m_deviceList;
  EgVehicleListData m_vehicleList;
};
#endif // EGDATAPROVIDER_H
