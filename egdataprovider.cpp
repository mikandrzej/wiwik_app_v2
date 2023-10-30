#include "egdataprovider.h"

EgDataProvider::EgDataProvider(QObject *parent) : QObject{parent} {
  m_mqttDataSource = new EgMqttDataSource(this);
  m_restDataSource = new EgRestDataSource(this);

  connect(m_restDataSource, &EgRestDataSource::vehicleListReady, this,
          &EgDataProvider::vehicleDataReceived);
  connect(m_restDataSource, &EgRestDataSource::deviceListReady, this,
          &EgDataProvider::devicesDataReceived);
  connect(m_mqttDataSource, &EgMqttDataSource::sensorDataReceived, this,
          &EgDataProvider::sensorLiveDataReceived);

  connect(m_restDataSource, &EgRestDataSource::vehicleListReady, this,
          [&](EgVehicleListData &vehicleListData) {
            this->m_vehicleList.vehicles.clear();
            this->m_vehicleList.vehicles.append(vehicleListData.vehicles);
          });
  connect(m_restDataSource, &EgRestDataSource::deviceListReady, this,
          [&](EgDevicesListData &vehicleListData) {
            this->m_deviceList.devices.clear();
            this->m_deviceList.devices.append(vehicleListData.devices);
          });

  connect(m_mqttDataSource, &EgMqttDataSource::serverStateChanged, this,
          &EgDataProvider::mqttServerStateChanged);
  connect(m_restDataSource, &EgRestDataSource::serverStateChanged, this,
          &EgDataProvider::restServerStateChanged);

  connect(m_restDataSource, &EgRestDataSource::vehiclesHistoryDataReady, this,
          &EgDataProvider::vehiclesHistoryDataReady);
  connect(this, &EgDataProvider::onAddNewVehicle, m_restDataSource,
          &EgRestDataSource::onAddNewVehicle);
}

void EgDataProvider::requestVehiclesData() {
  m_restDataSource->getVehicleList();
}

void EgDataProvider::requestDevicesData() {
  m_restDataSource->getDevicesList();
}

void EgDataProvider::assignVehicleToDevice(EgDeviceData *selectedDevice,
                                           EgVehicleData *selectedVehicle) {
  m_restDataSource->assignVehicleToDevice(selectedDevice, selectedVehicle);
}

void EgDataProvider::requestVehicleHistoryData(int vehicleId, QDate &date) {
  m_restDataSource->requestVehicleHistoryData(vehicleId, date);
}

const EgDevicesListData &EgDataProvider::getDevicesList() {
  return m_deviceList;
}

const EgVehicleListData &EgDataProvider::getVehiclesList() {
  return m_vehicleList;
}