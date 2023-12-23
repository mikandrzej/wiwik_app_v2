#ifndef EGDATAPROVIDER_H
#define EGDATAPROVIDER_H

#include "EgVehicleData.h"
#include "egmqttdatasource.h"
#include "egrestdatasource.h"
#include <QObject>

class EgDataProvider : public QObject
{
    Q_OBJECT
  public:
    explicit EgDataProvider(QObject* parent = nullptr);

    void requestVehiclesData();
    void requestDevicesData();
    void requestSensorsData();

  signals:
    void vehiclesDataReceived(EgVehiclesData& vehiclesData);
    void devicesDataReceived(EgDevicesListData& devicesData);
    void sensorsDataReceived(EgSensorsListData& sensorsData);
    void sensorLiveDataReceived(EgVehicleSensorData& sensorData);
    void mqttServerStateChanged(bool state);
    void restServerStateChanged(bool state);
    void vehiclesHistoryDataReady(EgTemperatureListData& tempListData);
    void onAddNewVehicle(QString& vehName, QString& plateNo);
    void onEditVehicle(int id, QString& vehName, QString& plateNo);

  public slots:
    void assignVehicleToDevice(EgDeviceData* selectedDevice, EgVehicleData* selectedVehicle);
    void requestVehicleHistoryData(int vehicleId, QDate& date);

  public:
    const EgDevicesListData& getDevicesList();
    const EgVehiclesData& getVehiclesList();

    EgMqttDataSource* mqttDataSource() const;

    EgRestDataSource* restDataSource() const;

  private:
    EgMqttDataSource* m_mqttDataSource;
    EgRestDataSource* m_restDataSource;
    EgDevicesListData m_deviceList;
    EgVehiclesData m_vehicleList;
};

#endif   // EGDATAPROVIDER_H
