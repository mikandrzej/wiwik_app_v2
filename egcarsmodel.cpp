#include "egcarsmodel.h"
#include <QColor>
#include <QDebug>

EgCarsModel::EgCarsModel(QObject *parent) : QAbstractTableModel{parent} {}

QVariant EgCarsModel::data(const QModelIndex &index, int role) const {

  if (index.row() < m_data.vehicles.count()) {
    auto data = m_data.vehicles[index.row()];
    switch (index.column()) {
    case EgCarModelColumns::Name:
      switch (role) {
      case Qt::DisplayRole:
        return data->name;
        break;
      }
      break;
    case EgCarModelColumns::PlateNo:
      switch (role) {
      case Qt::DisplayRole:
        return data->plateNo;
        break;
      }
      break;
    case EgCarModelColumns::Temperature:
      switch (role) {
      case Qt::DisplayRole:
        //        return QString("%1℃").arg(data->temperature);
        break;
      case Qt::BackgroundRole:
        //        if (data->temperature > 0.0)
        //            return QColorConstants::Red;
        //        else
        //            return QColorConstants::Green;
        break;
      }
      break;
    case EgCarModelColumns::BatteryVoltage:
      switch (role) {
      case Qt::DisplayRole:
        //        return QString("%1mV").arg(data->batteryVoltage);
        break;
      }
      break;
    case EgCarModelColumns::GPSPosition:
      switch (role) {
      case Qt::DisplayRole:
        //        return QString::number(data->gpsPosition.latitude(), 'f', 6) +
        //        ", " +
        //               QString::number(data->gpsPosition.longitude(), 'f', 6);
        break;
      }
      break;
    }
  }
  return QVariant();
}

void EgCarsModel::appendCar(EgVehicleData *car) {
  beginInsertRows(QModelIndex(), m_data.vehicles.count(),
                  m_data.vehicles.count());
  m_data.vehicles.append(car);
  endInsertRows();
}

void EgCarsModel::overrideCars(EgVehiclesData &list) {
  beginResetModel();
  m_data.vehicles.clear();
  m_data.vehicles.append(list.vehicles);
  endResetModel();
}

void EgCarsModel::onSensorDataReceived(EgSensorData &sensorData) {
  for (int k = 0; k < m_data.vehicles.count(); k++) {
    auto &vehicle = m_data.vehicles[k];
    if (vehicle->id == sensorData.vehicleId) {
      switch (sensorData.dataType) {
      case EgSensorDataType::Temperature1: {
        //        vehicle->temperature = sensorData.temperature;
        emit dataChanged(createIndex(k, EgCarModelColumns::Temperature),
                         createIndex(k, EgCarModelColumns::Temperature),
                         {Qt::DisplayRole});

        break;
      }
      case EgSensorDataType::BatteryVoltage: {
        //        vehicle->batteryVoltage = sensorData.battery;
        emit dataChanged(createIndex(k, EgCarModelColumns::BatteryVoltage),
                         createIndex(k, EgCarModelColumns::BatteryVoltage),
                         {Qt::DisplayRole});
        break;
      }
      case EgSensorDataType::GpsPosition:
        //        vehicle->gpsPosition = sensorData.geoPosition.coordinate();
        emit dataChanged(createIndex(k, EgCarModelColumns::GPSPosition),
                         createIndex(k, EgCarModelColumns::GPSPosition),
                         {Qt::DisplayRole});
        break;
      }
    }
  }
}

const EgVehiclesData &EgCarsModel::getData() { return m_data; }

bool EgCarsModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (index.isValid()) {
    if (Qt::DisplayRole == role) {
      switch (index.column()) {
      case EgCarModelColumns::Name:
        m_data.vehicles[index.row()]->name = value.toString();
        emit dataChanged(index, index, {role});
        return true;
      case EgCarModelColumns::PlateNo:
        m_data.vehicles[index.row()]->plateNo = value.toString();
        emit dataChanged(index, index, {role});
        return true;
      }
    }
  }
  return false;
}

int EgCarsModel::rowCount(const QModelIndex &parent) const {
  return m_data.vehicles.count();
}

int EgCarsModel::columnCount(const QModelIndex &parent) const {
  return EgCarModelColumns::Length;
}

QVariant EgCarsModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
  switch (orientation) {
  case Qt::Horizontal:
    switch (section) {
    case EgCarModelColumns::Name:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Pojazd");
      }
      break;
    case EgCarModelColumns::PlateNo:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Rejestracja");
      }
      break;
    case EgCarModelColumns::Temperature:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Temperatura");
      }
      break;
    case EgCarModelColumns::BatteryVoltage:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Napięcie baterii");
      }
      break;
    case EgCarModelColumns::GPSPosition:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Pozycja GPS");
      }
      break;
    }
    break;
  case Qt::Vertical:
    break;
  }
  return QVariant();
}
