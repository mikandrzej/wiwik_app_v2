#include "vehicletreemodel.h"

VehicleTreeModel::VehicleTreeModel(QObject *parent)
    : QAbstractItemModel{parent} {}

QModelIndex VehicleTreeModel::index(int row, int column,
                                    const QModelIndex &parent) const {
  return QModelIndex();
}

QModelIndex VehicleTreeModel::parent(const QModelIndex &child) const {
  return QModelIndex();
}

int VehicleTreeModel::rowCount(const QModelIndex &parent) const { return 0; }

int VehicleTreeModel::columnCount(const QModelIndex &parent) const { return 0; }

QVariant VehicleTreeModel::data(const QModelIndex &index, int role) const {
  return QModelIndex();
}
