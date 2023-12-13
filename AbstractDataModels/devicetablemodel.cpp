#include "devicetablemodel.h"

#include "../DataSource/datacontainer.h"

#include "../DataModels/device.h"
#include "../DataModels/vehicle.h"
#include "QtGui/qcolor.h"

DeviceTableModel deviceTableModel;

DeviceTableModel::DeviceTableModel(QObject *parent)
    : QAbstractTableModel{parent} {
  connect(DataContainer::instance(), &DataContainer::deviceDataToBeInserted,
          this, [this](int row) { beginInsertRows(QModelIndex(), row, row); });
  connect(DataContainer::instance(), &DataContainer::deviceDataInsertDone, this,
          [this]() { endInsertRows(); });
  connect(DataContainer::instance(), &DataContainer::deviceDataChanged, this,
          [this](int idx) {
            emit dataChanged(
                createIndex(idx, 0), createIndex(idx, Columns::ColumnMax - 1),
                {Qt::DisplayRole, Qt::CheckStateRole, Qt::BackgroundRole});
          });
}

int DeviceTableModel::rowCount(const QModelIndex &parent) const {
  return DataContainer::instance()->getDevicesCount();
}

int DeviceTableModel::columnCount(const QModelIndex &parent) const {
  return static_cast<int>(ColumnMax);
}

QVariant DeviceTableModel::data(const QModelIndex &index, int role) const {
  const auto *device = DataContainer::instance()->getDeviceByIndex(index.row());
  if (nullptr == device)
    return QVariant();

  const auto column = index.column();
  if (Qt::DisplayRole == role || Qt::EditRole == role) {
    switch (static_cast<Columns>(column)) {
    case ColumnId:
      return device->id();
    case ColumnSerialNo:
      return device->serialNo();
    case ColumnName:
      return device->name();
    case ColumnVehicle: {
      const auto *vehicle =
          DataContainer::instance()->getVehicleById(device->vehicleId());
      if (vehicle != nullptr)
        return QString::number(vehicle->id()) + ": " + vehicle->name();
      return "Nie przypisano";
    }
    case ColumnType:
      return device->type();
    }
  } else if (Qt::BackgroundRole == role) {
    switch (static_cast<Columns>(column)) {
    case ColumnCommit:
      if (device->changesPending()) {
        return QColorConstants::Magenta;
      }
    }
  } else if (Qt::CheckStateRole == role) {
    switch (static_cast<Columns>(column)) {
    case ColumnCommit:
      if (device->changesPending())
        return !device->changesPending();
    }
  }
  return QVariant();
}

bool DeviceTableModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
  auto *device = DataContainer::instance()->getDeviceByIndex(index.row());
  if (nullptr == device)
    return false;

  const auto column = index.column();
  if (Qt::EditRole == role) {
    switch (static_cast<Columns>(column)) {
    case ColumnName:
      device->setName(value.toString());
      return true;
    case ColumnVehicle: {
      bool ok;
      auto id = value.toInt(&ok);
      if (ok)
        device->setVehicleId(id);
      return ok;
    }
    }
  } else if (Qt::CheckStateRole == role) {
    if (ColumnCommit == static_cast<Columns>(column)) {
      device->commitChanges();
    }
  }

  return false;
}

QVariant DeviceTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (m_columnData.contains(section)) {
    if (Qt::Orientation::Horizontal == orientation) {
      if (Qt::DisplayRole == role) {
        return m_columnData[section].name;
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags DeviceTableModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  const auto *device = DataContainer::instance()->getDeviceByIndex(index.row());
  switch (static_cast<Columns>(index.column())) {
  case ColumnName:
  case ColumnVehicle:
    flags |= Qt::ItemIsEditable;
    break;
  case ColumnCommit:
    if (device)
      if (device->changesPending())
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    break;
  }

  return flags;
}

void DeviceTableModel::onDataReset() {
  beginResetModel();
  endResetModel();
}
