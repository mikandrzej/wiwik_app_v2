#include "liveviewmodel.h"
#include "DataSource/EgVehicleData.h"
#include <QColor>
#include <QDebug>
#include <QIcon>
#include <QStringList>

LiveViewModel::LiveViewModel(QObject *parent) : QAbstractItemModel(parent) {
  QList<TreeItemData *> rootData;
  auto item = new TreeItemData();
  item->text = "Pojazd";
  item->font = new QFont();
  item->font->setBold(true);
  rootData.append(item);
  item = new TreeItemData();
  item->text = "Dane";
  item->font = new QFont();
  item->font->setBold(true);
  rootData.append(item);

  rootItem = new TreeItem(rootData);
}

LiveViewModel::~LiveViewModel() { delete rootItem; }

int LiveViewModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant LiveViewModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
  auto *data = item->data(index.column());
  if (!data)
    return QVariant();

  switch (role) {
  case Qt::DisplayRole:
    return data->text;
  case Qt::FontRole: {
    if (data->font)
      return *data->font;
    break;
  }
  case Qt::DecorationRole: {
    if (data->decoration)
      return *data->decoration;
    break;
  }
  case Qt::UserRole:
    return QVariant::fromValue(item);

  default:
    return QVariant();
  }
  return QVariant();
}

Qt::ItemFlags LiveViewModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemFlags();

  return QAbstractItemModel::flags(index);
}

QVariant LiveViewModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const {
  if (orientation == Qt::Horizontal) {
    auto *data = rootItem->data(section);
    if (!data)
      return QVariant();
    switch (role) {
    case Qt::DisplayRole:
      return data->text;
    case Qt::FontRole: {
      if (data->font)
        return *data->font;
      break;
    }
    case Qt::DecorationRole: {
      if (data->decoration)
        return *data->decoration;
      break;
    }
    default:
      return QVariant();
    }
    return QVariant();
  }

  return QVariant();
}

QModelIndex LiveViewModel::index(int row, int column,
                                 const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  TreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem *>(parent.internalPointer());

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex LiveViewModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());
  TreeItem *parentItem = childItem->parentItem();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int LiveViewModel::rowCount(const QModelIndex &parent) const {
  TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem *>(parent.internalPointer());

  return parentItem->childCount();
}

int LiveViewModel::findNode(unsigned int &hash,
                            const QList<TreeItem *> &tList) {
  for (int idx = 0; idx < tList.size(); ++idx) {
    unsigned int z = tList.at(idx)->getIndex();
    if (z == hash)
      return idx;
  }

  return -1;
}

void LiveViewModel::onVehiclesDataReceived(EgVehiclesData &vehiclesData) {
  beginResetModel();
  rootItem->clear();
  vehicleItem.clear();
  for (auto &vehicle : vehiclesData.vehicles) {
    QList<TreeItemData *> columnData;
    auto *column = new TreeItemData();
    column->text = vehicle->name;
    column->font = new QFont();
    column->font->setBold(true);
    //    column->decoration = new QVariant(QColorConstants::Blue);
    column->decoration = new QVariant(QIcon(":/icons8-car-50.png"));
    columnData << column;
    column = new TreeItemData();
    column->text = vehicle->plateNo;
    columnData << column;
    auto ti = new TreeItem(columnData, rootItem,
                           static_cast<unsigned int>(vehicle->id));
    vehicleItem[vehicle->id] = ti;
    rootItem->appendChild(ti);
  }

  endResetModel();
}

void LiveViewModel::onDevicesDataReceived(EgDevicesListData &devicesData) {
  beginResetModel();
  for (auto &device : devicesData.devices) {
    auto vehicle_node = vehicleItem[device->vehicle_id];
    if (!vehicle_node) {
      qDebug() << "Could not find vehicle with id: " << device->vehicle_id;
      continue;
    }

    QString device_name = device->name;
    if (device_name.length() == 0) {
      device_name = device->serial_no;
    }

    QList<TreeItemData *> columnData;
    auto *column = new TreeItemData();
    column->text = device_name;
    columnData << column;
    column = new TreeItemData();
    column->text = "";
    columnData << column;
    auto ti = new TreeItem(columnData, vehicle_node,
                           static_cast<unsigned int>(device->id));
    deviceItem[device->id] = ti;
    vehicle_node->appendChild(ti);
  }
  endResetModel();
}

void LiveViewModel::onSensorsDataReceived(EgSensorsListData &sensorsData) {
  beginResetModel();
  for (auto &sensor : sensorsData.sensors) {
    auto device_node = deviceItem[sensor->device_id];
    if (!device_node) {
      qDebug() << "Could not find device with id: " << sensor->device_id;
      continue;
    }

    QString sensor_name = sensor->name;
    if (sensor_name.length() == 0) {
      sensor_name = sensor->address;
    }

    QList<TreeItemData *> columnData;
    auto *column = new TreeItemData();
    if (sensor->type == "temperature") {
      column->decoration = new QVariant(QIcon(":/icons8-temperature-50.png"));
    } else if (sensor->type == "battery") {
      column->decoration = new QVariant(QIcon(":/icons8-battery-60.png"));
    } else if (sensor->type == "gps") {
      column->decoration = new QVariant(QIcon(":/icons8-gps-50.png"));
    }
    column->text = sensor_name;
    columnData << column;
    column = new TreeItemData();
    column->text = "";
    columnData << column;
    auto ti = new TreeItem(columnData, device_node,
                           static_cast<unsigned int>(sensor->id + 100));
    sensorItem[sensor->address] = ti;
    device_node->appendChild(ti);
  }
  endResetModel();
}

void LiveViewModel::onSensorLiveDataReceived(EgVehicleSensorData &sensorData) {

  auto *sensor_node = sensorItem[sensorData.sensorAddress];
  if (!sensor_node) {
    qDebug() << "Could not find device with id: " << sensorData.sensorAddress;
    return;
  }

  switch (sensorData.dataType) {
  case EgSensorDataType::Temperature:
    sensor_node->data(1)->text =
        QString::number(sensorData.temperature, 10, 2) + "'C";
    break;
  case EgSensorDataType::BatteryVoltage:
    sensor_node->data(1)->text =
        QString::number(sensorData.battery / 1000.0, 10, 2) + "V";
    break;
  case EgSensorDataType::GpsPosition:
    sensor_node->data(1)->text = sensorData.address;
    break;
  }

  QModelIndex foundIdx = findIndex(sensor_node, 1);
  if (foundIdx.isValid()) {
    emit dataChanged(foundIdx, foundIdx, {Qt::DisplayRole});
  }
}

QModelIndex LiveViewModel::findIndex(TreeItem *ptr, int column,
                                     const QModelIndex &parent) {
  const int row_cnt = rowCount(parent);
  for (int i = 0; i < row_cnt; ++i) {
    auto idx = index(i, 0, parent);
    auto treeItem = static_cast<TreeItem *>(idx.internalPointer());
    if (treeItem == ptr)
      return idx.siblingAtColumn(column);
    auto childIdx = findIndex(ptr, column, idx);
    if (childIdx.isValid())
      return childIdx;
  }

  return QModelIndex();
}
