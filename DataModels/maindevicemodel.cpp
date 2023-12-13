#include "maindevicemodel.h"

MainDeviceModel::MainDeviceModel(QObject *parent)
    : QAbstractItemModel{parent} {}

QModelIndex MainDeviceModel::index(int row, int column,
                                   const QModelIndex &parent) const {}

QModelIndex MainDeviceModel::parent(const QModelIndex &child) const {}

int MainDeviceModel::rowCount(const QModelIndex &parent) const {}

int MainDeviceModel::columnCount(const QModelIndex &parent) const {}

QVariant MainDeviceModel::data(const QModelIndex &index, int role) const {}
