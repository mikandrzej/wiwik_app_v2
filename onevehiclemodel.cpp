#include "onevehiclemodel.h"

OneVehicleModel::OneVehicleModel(QObject *parent) : QObject{parent} {}

int OneVehicleModel::id() const { return m_id; }

void OneVehicleModel::setId(int newId) {
  if (m_id == newId)
    return;
  m_id = newId;
  emit idChanged();
}

QString OneVehicleModel::name() const { return m_name; }

void OneVehicleModel::setName(const QString &newName) {
  if (m_name == newName)
    return;
  m_name = newName;
  emit nameChanged();
}

QString OneVehicleModel::plateNo() const { return m_plateNo; }

void OneVehicleModel::setPlateNo(const QString &newPlateNo) {
  if (m_plateNo == newPlateNo)
    return;
  m_plateNo = newPlateNo;
  emit plateNoChanged();
}

OneVehicleTemperatureModel *OneVehicleModel::temperatureModel() const {
  return m_temperatureModel;
}

OneVehiclePositionModel *OneVehicleModel::positionModel() const {
  return m_position;
}

OneVehicleBatteryModel *OneVehicleModel::batteryModel() const {
  return m_battery;
}
