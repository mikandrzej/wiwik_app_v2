#include "vehicleeditmodel.h"

VehicleEditModel::VehicleEditModel(QObject *parent)
    : QAbstractProxyModel{parent} {}

QModelIndex VehicleEditModel::index(int row, int column,
                                    const QModelIndex &parent) const {
  return sourceModel()->index(row, column, parent);
}

QModelIndex VehicleEditModel::parent(const QModelIndex &child) const {
  return sourceModel()->parent(child);
}

int VehicleEditModel::rowCount(const QModelIndex &parent) const {
  return sourceModel()->rowCount(parent);
}

int VehicleEditModel::columnCount(const QModelIndex &parent) const {
  return sourceModel()->columnCount(parent);
}

QModelIndex VehicleEditModel::mapToSource(const QModelIndex &proxyIndex) const {
  return sourceModel()->index(proxyIndex.row(), proxyIndex.column(),
                              proxyIndex.parent());
}

QModelIndex
VehicleEditModel::mapFromSource(const QModelIndex &sourceIndex) const {
  return sourceModel()->index(sourceIndex.row(), sourceIndex.column(),
                              sourceIndex.parent());
}
