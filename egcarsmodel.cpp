#include "egcarsmodel.h"
#include <QColor>
#include <QDebug>

EgCarsModel::EgCarsModel(QObject *parent) : QAbstractTableModel{parent} {}

QVariant EgCarsModel::data(const QModelIndex &index, int role) const {

  if (index.row() < m_data.vehicles.count()) {
    auto data = m_data.vehicles[index.row()];
    switch (index.column()) {
    case 0:
      switch (role) {
      case Qt::DisplayRole:
        return data->name;
        break;
      }
      break;
    case 1:
      switch (role) {
      case Qt::DisplayRole:
        return data->plateNo;
        break;
      }
      break;
    case 2:
      switch (role) {
      case Qt::DisplayRole:
        return QString("%1℃").arg(data->temperature);
        break;
      case Qt::BackgroundRole:
        if (data->temperature > 0.0)
          return QColorConstants::Red;
        else
          return QColorConstants::Green;
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

void EgCarsModel::overrideCars(EgVehicleListData &list) {
  beginResetModel();
  m_data.vehicles.clear();
  m_data.vehicles.append(list.vehicles);
  endResetModel();
}

void EgCarsModel::onSensorDataReceived(EgSensorData &sensorData) {
  for (int k = 0; k < m_data.vehicles.count(); k++) {
    auto &vehicle = m_data.vehicles[k];
    if (vehicle->id == sensorData.vehicleId) {
      qDebug() << "Update sensor data with temperature: "
               << sensorData.temperature;
      vehicle->temperature = sensorData.temperature;
      emit dataChanged(createIndex(k, 2), createIndex(k, 2), {Qt::DisplayRole});
    }
  }
}

const EgVehicleListData &EgCarsModel::getData() { return m_data; }

bool EgCarsModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (index.isValid()) {
    if (Qt::DisplayRole == role) {
      switch (index.column()) {
      case 0:
        m_data.vehicles[index.row()]->name = value.toString();
        emit dataChanged(index, index, {role});
        return true;
      case 1:
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

int EgCarsModel::columnCount(const QModelIndex &parent) const { return 3; }

QVariant EgCarsModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
  switch (orientation) {
  case Qt::Horizontal:
    switch (section) {
    case 0:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Pojazd");
      }
    case 1:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Rejestracja");
      }
    case 2:
      switch (role) {
      case Qt::DisplayRole:
        return QString("Temperatura");
      }
    }
    break;
  case Qt::Vertical:
    break;
  }
  return QVariant();
}
