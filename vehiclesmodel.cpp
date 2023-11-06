#include "vehiclesmodel.h"
#include "math.h"
#include <QSet>

VehiclesModel::VehiclesModel(QObject *parent) : QAbstractTableModel{parent} {}

int VehiclesModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_vehicles.count();
}

int VehiclesModel::columnCount(const QModelIndex &parent) const {
  return ColumnMax;
}

QVariant VehiclesModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  auto *item = m_vehicles[index.row()];

  switch (index.column()) {
  case ColumnId:
    switch (role) {
    case Qt::DisplayRole:
      return item->id();
    }
    break;
  case ColumnName:
    switch (role) {
    case Qt::DisplayRole:
      return item->name();
    }
    break;
  case ColumnPlateNo:
    switch (role) {
    case Qt::DisplayRole:
      return item->plateNo();
    }
    break;
  case ColumnTemperature:
    switch (role) {
    case Qt::DisplayRole: {
      auto value = item->temperatureModel()->lastValue();
      if (isnan(value))
        return "-";
      else
        return QString("%1℃").arg(QString::number(value, 'f', 2));
    }
    }
    break;
  case ColumnPosition:
    switch (role) {
    case Qt::DisplayRole: {
      auto position = item->positionModel()->lastValue();
      if (position.isValid()) {
        return QString("Lat:%1 Long:%2")
            .arg(QString::number(position.coordinate().latitude()),
                 QString::number(position.coordinate().longitude()));
      }
    }
    }
    break;
  case ColumnBattery:
    switch (role) {
    case Qt::DisplayRole: {
      auto value = item->batteryModel()->lastValue();
      if (isnan(value))
        return "-";
      else
        return QString("%1mV").arg(QString::number(value, 'f', 2));
    }
    }
    break;
  }
  return QVariant();
}

QVariant VehiclesModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const {
  if (Qt::Horizontal == orientation) {
    if (Qt::DisplayRole == role) {
      switch (section) {
      case ColumnId:
        return "ID";
      case ColumnName:
        return "Nazwa";
      case ColumnPlateNo:
        return "Nr rej.";
      case ColumnTemperature:
        return "Temperatura";
      case ColumnPosition:
        return "Pozycja";
      case ColumnBattery:
        return "Bateria";
      default:
        return QVariant();
      }
    } else {
      return QVariant();
    }
  }
  return QVariant();
}

QHash<int, QByteArray> VehiclesModel::roleNames() const {
  return QHash<int, QByteArray>{{RoleHistoryData, "historyData"}};
}

void VehiclesModel::onSensorDataReceived(EgSensorData &sensorData) {
  if (!m_vehiclesById.contains(sensorData.vehicleId))
    return;

  auto model = m_vehiclesById[sensorData.vehicleId];
  auto row = m_vehicleRowByModel[model];

  switch (sensorData.dataType) {
  case EgSensorDataType::Temperature1:
    model->temperatureModel()->insert(sensorData.timestamp.toSecsSinceEpoch(),
                                      sensorData.temperature);
    emit dataChanged(index(row, ColumnTemperature),
                     index(row, ColumnTemperature));
    break;
  case EgSensorDataType::BatteryVoltage:
    model->batteryModel()->insert(sensorData.timestamp.toSecsSinceEpoch(),
                                  sensorData.battery);
    emit dataChanged(index(row, ColumnBattery), index(row, ColumnBattery));
    break;
  case EgSensorDataType::GpsPosition:
    model->positionModel()->insert(sensorData.timestamp.toSecsSinceEpoch(),
                                   sensorData.geoPosition);
    emit dataChanged(index(row, ColumnPosition), index(row, ColumnPosition));
    break;
  }
}

void VehiclesModel::onSensorsDataReceived(EgSensorsData &sensorsData) {
  for (auto sensorData : sensorsData.sensors) {
    auto model = m_vehiclesById[sensorData->vehicleId];
    switch (sensorData->dataType) {
    case EgSensorDataType::Temperature1:
      model->temperatureModel()->insert(
          sensorData->timestamp.toSecsSinceEpoch(), sensorData->temperature);
      break;
    case EgSensorDataType::BatteryVoltage:
      model->batteryModel()->insert(sensorData->timestamp.toSecsSinceEpoch(),
                                    sensorData->battery);
      break;
    case EgSensorDataType::GpsPosition:
      model->positionModel()->insert(sensorData->timestamp.toSecsSinceEpoch(),
                                     sensorData->geoPosition);
      break;
    }
  }
}

void VehiclesModel::onVehiclesDataReceived(EgVehiclesData &vehiclesData) {
  QSet<int> receivedVehiclesId;

  for (auto &vehicle : vehiclesData.vehicles) {
    receivedVehiclesId.insert(vehicle->id);

    OneVehicleModel *model;
    if (!m_vehiclesById.contains(vehicle->id)) {
      model = new OneVehicleModel();
      // todo add in the middle?

      auto newRowId = m_vehicles.count();
      beginInsertRows(QModelIndex(), newRowId, newRowId);

      // add to array
      m_vehicles.append(model);
      // add to lookups
      m_vehiclesById[vehicle->id] = model;
      m_vehicleRowByModel[model] = newRowId;

      endInsertRows();
    } else {
      model = m_vehiclesById[vehicle->id];
    }

    model->setId(vehicle->id);
    model->setName(vehicle->name);
    model->setPlateNo(vehicle->plateNo);
  }

  // find removed vehicles
  QList<int> removedVehicles;
  for (auto &model : m_vehicles) {
    if (!receivedVehiclesId.contains(model->id()))
      removedVehicles.append(model->id());
  }
  // remove vehicles
  // todo
}
