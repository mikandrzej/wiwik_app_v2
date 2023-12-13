#include "datacontainer.h"

#include "../DataModels/device.h"
#include "../DataModels/sensor.h"
#include "../DataModels/vehicle.h"

#include "egdataprovider.h"

DataContainer *dataContainer;

DataContainer::DataContainer(QObject *parent) : QObject{parent} {}

void DataContainer::overwriteVehicles(QList<Vehicle *> &vehicles) {
  QMap<int, Vehicle *> newVehiclesMap;
  for (auto *element : vehicles) {
    if (m_vehiclesMap.contains(element->id())) {
      // udpate exisiting vehicle
      updateVehicle(element);
    } else {
      // add new vehicle
      addVehicle(element);
    }

    newVehiclesMap.insert(element->id(), element);
  }

  // remove vehicles not found in new data
  QList<int> vehiclesToDelete;
  for (auto *existVehicle : qAsConst(m_vehiclesMap)) {
    if (!newVehiclesMap.contains(existVehicle->id())) {
      // remove vehicle from map
      vehiclesToDelete.append(existVehicle->id());
    }
  }

  for (int vehicleId : vehiclesToDelete) {
    removeVehicle(vehicleId);
  }
}

void DataContainer::overwriteDevices(QList<Device *> &devices) {
  QMap<int, Device *> newDevicesMap;
  for (auto *element : devices) {
    if (m_devicesMap.contains(element->id())) {
      // udpate exisiting device
      updateDevice(element);
    } else {
      // add new device
      addDevice(element);
    }

    newDevicesMap.insert(element->id(), element);
  }

  // remove devices not found in new data
  QList<int> devicesToDelete;
  for (auto *existDevice : qAsConst(m_devicesMap)) {
    if (!newDevicesMap.contains(existDevice->id())) {
      // remove device from map
      devicesToDelete.append(existDevice->id());
    }
  }

  for (int deviceId : devicesToDelete) {
    removeDevice(deviceId);
  }
}

void DataContainer::overwriteSensors(QList<Sensor *> &sensors) {
  QMap<int, Sensor *> newSensorsMap;
  for (auto *element : sensors) {
    if (m_sensorsMap.contains(element->id())) {
      // udpate exisiting sensor
      updateSensor(element);
    } else {
      // add new sensor
      addSensor(element);
    }

    newSensorsMap.insert(element->id(), element);
  }

  // remove sensors not found in new data
  QList<int> sensorsToDelete;
  for (auto *existSensor : qAsConst(m_sensorsMap)) {
    if (!newSensorsMap.contains(existSensor->id())) {
      // remove sensor from map
      sensorsToDelete.append(existSensor->id());
    }
  }

  for (int sensorId : sensorsToDelete) {
    removeSensor(sensorId);
  }
}

int DataContainer::getVehiclesCount() { return m_vehiclesList.count(); }

int DataContainer::getDevicesCount() { return m_devicesList.count(); }

int DataContainer::getSensorsCount() { return m_sensorsList.count(); }

QList<Vehicle *> DataContainer::getVehicleList() { return m_vehiclesList; }

QList<Device *> DataContainer::getDeviceList() { return m_devicesList; }

Vehicle *DataContainer::getVehicleByIndex(int index) {
  if (index >= m_vehiclesList.count())
    return nullptr;
  return m_vehiclesList.at(index);
}

int DataContainer::getIndexByVehicle(Vehicle *vehicle) {
  if (!m_vehiclesList.contains(vehicle))
    return -1;
  return m_vehiclesList.indexOf(vehicle);
}

Vehicle *DataContainer::getVehicleById(int id) {
  if (m_vehiclesMap.contains(id))
    return m_vehiclesMap[id];
  return nullptr;
}

Device *DataContainer::getDeviceByIndex(int index) {
  if (index >= m_devicesList.count())
    return nullptr;
  return m_devicesList.at(index);
}

int DataContainer::getIndexByDevice(Device *device) {
  if (!m_devicesList.contains(device))
    return -1;
  return m_devicesList.indexOf(device);
}

Device *DataContainer::getDeviceById(int id) {
  if (m_devicesMap.contains(id))
    return m_devicesMap[id];
  return nullptr;
}

Sensor *DataContainer::getSensorByIndex(int index) {

  if (index >= m_sensorsList.count())
    return nullptr;
  return m_sensorsList.at(index);
}

int DataContainer::getIndexBySensor(Sensor *sensor) {
  if (!m_sensorsList.contains(sensor))
    return -1;
  return m_sensorsList.indexOf(sensor);
}

Sensor *DataContainer::getSensorById(int id) {
  if (m_sensorsMap.contains(id))
    return m_sensorsMap[id];
  return nullptr;
}

Sensor *DataContainer::getSensorByAddress(QString &address) {
  if (m_sensorsAddressMap.contains(address))
    return m_sensorsAddressMap[address];
  return nullptr;
}

void DataContainer::insertNewMeasure(QString &sensorAddress, Measure &measure)
{
    if(m_sensorsAddressMap.contains(sensorAddress))
    {
        m_sensorsAddressMap[sensorAddress]->addMeasure(measure);
    }
    else
    {
        emit sensorsDataNeedsUpdate();
    }
}

DataContainer *DataContainer::instance() {
  if (nullptr == dataContainer)
    dataContainer = new DataContainer();
  return dataContainer;
}

void DataContainer::removeDevice(int id) {
  m_devicesMap.remove(id);

  m_devicesList = m_devicesMap.values();
}

void DataContainer::addDevice(Device *device) {
  auto newDevice = new Device(*device, this);

  if(m_vehiclesMap.contains(device->vehicleId()))
  {
      newDevice->setVehicle(m_vehiclesMap[device->vehicleId()]);
      newDevice->vehicle()->addDevice(newDevice);
  }
  for(auto *sensor : qAsConst(m_sensorsList))
  {
      if(sensor->deviceId() == newDevice->id())
      {
          sensor->setDevice(newDevice);
          newDevice->addSensor(sensor);
      }
  }

  int newIdx = -1;
  int newDeviceId = device->id();
  auto storedIds = m_devicesMap.keys();
  for (int k = 0; k < storedIds.count(); k++) {
    if (newDeviceId < storedIds[k]) {
      newIdx = k;
      break;
    }
  }
  if (newIdx < 0)
    newIdx = m_devicesMap.count();

  emit deviceDataToBeInserted(newIdx);

  m_devicesMap.insert(newDevice->id(), newDevice);

  m_devicesList = m_devicesMap.values();

  connect(newDevice, &Device::changesPendingChanged, this, [this, newDevice]() {
    emit deviceDataChanged(m_devicesList.indexOf(newDevice));
  });
  connect(newDevice, &Device::nameChanged, this, [this, newDevice]() {
    emit deviceDataChanged(m_devicesList.indexOf(newDevice));
  });
  connect(newDevice, &Device::vehicleIdChanged, this, [this, newDevice]() {
      if(newDevice->vehicle() != nullptr)
      {
        newDevice->vehicle()->removeDevice(newDevice);
      }
      if(m_vehiclesMap.contains(newDevice->vehicleId()))
      {
          newDevice->setVehicle(m_vehiclesMap[newDevice->vehicleId()]);
          newDevice->vehicle()->addDevice(newDevice);
      }

    emit deviceDataChanged(m_devicesList.indexOf(newDevice));
  });

  emit deviceDataInsertDone();
}

void DataContainer::updateDevice(Device *device) {
  auto *existDevice = m_devicesMap[device->id()];
    if(existDevice != nullptr)
  {
      existDevice->setName(device->name());
      existDevice->setVehicleId(device->vehicleId());
  }
}

void DataContainer::updateVehicle(Vehicle *vehicle) {
  auto *existVehicle = m_vehiclesMap[vehicle->id()];
    if(existVehicle != nullptr)
  {
      existVehicle->setName(vehicle->name());
      existVehicle->setPlateNo(vehicle->plateNo());
  }
}

void DataContainer::addVehicle(Vehicle *vehicle) {
  auto newVehicle = new Vehicle(*vehicle, this);

  for(auto *device : qAsConst(m_devicesList))
  {
      if(device->vehicleId() == newVehicle->id())
      {
          device->setVehicle(newVehicle);
          newVehicle->addDevice(device);
      }
  }

  int newIdx = -1;
  int newVehicleId = vehicle->id();
  auto storedIds = m_vehiclesMap.keys();
  for (int k = 0; k < storedIds.count(); k++) {
    if (newVehicleId < storedIds[k]) {
      newIdx = k;
      break;
    }
  }
  if (newIdx < 0)
    newIdx = m_vehiclesMap.count();

  emit vehicleDataToBeInserted(newIdx);

  m_vehiclesMap.insert(newVehicle->id(), newVehicle);

  m_vehiclesList = m_vehiclesMap.values();

  connect(newVehicle, &Vehicle::changesPendingChanged, this,
          [this, newVehicle]() {
            emit vehicleDataChanged(m_vehiclesList.indexOf(newVehicle));
          });
  connect(newVehicle, &Vehicle::nameChanged, this, [this, newVehicle]() {
    emit vehicleDataChanged(m_vehiclesList.indexOf(newVehicle));
  });
  connect(newVehicle, &Vehicle::plateNoChanged, this, [this, newVehicle]() {
      emit vehicleDataChanged(m_vehiclesList.indexOf(newVehicle));
  });

  emit vehicleDataInsertDone();
}

void DataContainer::removeVehicle(int id) {
  m_vehiclesMap.remove(id);

  m_vehiclesList = m_vehiclesMap.values();
}

void DataContainer::updateSensor(Sensor *sensor) {
  auto *existSensor = m_sensorsMap[sensor->id()];
    if(existSensor != nullptr)
  {
      existSensor->setName(sensor->name());
      existSensor->setDeviceId(sensor->deviceId());
  }
}

void DataContainer::removeSensor(int id) {
    auto *sensor = m_sensorsMap[id];
    if(sensor != nullptr)
    {
        if(sensor->device() != nullptr)
        {
            sensor->device()->removeSensor(sensor);
        }
        m_sensorsAddressMap.remove(m_sensorsMap[id]->address());
        m_sensorsMap.remove(id);

        m_sensorsList = m_sensorsMap.values();
    }
}

void DataContainer::addSensor(Sensor *sensor) {
  auto newSensor = new Sensor(*sensor, this);
    if(m_devicesMap.contains(sensor->deviceId()))
  {
    newSensor->setDevice(m_devicesMap[sensor->deviceId()]);
    newSensor->device()->addSensor(newSensor);
  }

  int newIdx = -1;
  int newSensorId = sensor->id();
  auto storedIds = m_sensorsMap.keys();
  for (int k = 0; k < storedIds.count(); k++) {
    if (newSensorId < storedIds[k]) {
      newIdx = k;
      break;
    }
  }
  if (newIdx < 0)
    newIdx = m_sensorsMap.count();

  emit sensorDataToBeInserted(newIdx);

  m_sensorsMap.insert(newSensor->id(), newSensor);
  m_sensorsAddressMap.insert(newSensor->address(), newSensor);

  m_sensorsList = m_sensorsMap.values();

  connect(newSensor, &Sensor::changesPendingChanged, this, [this, newSensor]() {
    emit sensorDataChanged(m_sensorsList.indexOf(newSensor));
  });
  connect(newSensor, &Sensor::nameChanged, this, [this, newSensor]() {
      emit sensorDataChanged(m_sensorsList.indexOf(newSensor));
  });
  connect(newSensor, &Sensor::deviceIdChanged, this, [this, newSensor]() {
      if(newSensor->device() != nullptr)
      {
        newSensor->device()->removeSensor(newSensor);
      }
      if(m_devicesMap.contains(newSensor->deviceId()))
      {
        newSensor->setDevice(m_devicesMap[newSensor->deviceId()]);
        newSensor->device()->addSensor(newSensor);
      }

      emit sensorDataChanged(m_sensorsList.indexOf(newSensor));
  });
  connect(newSensor, &Sensor::lastMeasureChanged, this, [this, newSensor]() {
    emit sensorDataChanged(m_sensorsList.indexOf(newSensor));
  });

  emit sensorDataInsertDone();
}
