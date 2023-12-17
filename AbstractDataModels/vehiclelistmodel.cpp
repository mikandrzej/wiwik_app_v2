#include "vehiclelistmodel.h"

#include "../DataModels/vehicle.h"
#include "../DataSource/datacontainer.h"

VehicleListModel vehicleListModel;

VehicleListModel::VehicleListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    connect(DataContainer::instance(),
            &DataContainer::vehicleDataToBeInserted,
            this,
            [this](int row) { beginInsertRows(QModelIndex(), row, row); });
    connect(DataContainer::instance(), &DataContainer::vehicleDataInsertDone, this, [this]() {
        endInsertRows();
    });
    connect(DataContainer::instance(), &DataContainer::vehicleDataChanged, this, [this](int idx) {
        auto index = createIndex(idx, 0);
        emit dataChanged(index, index, {Qt::DisplayRole});
    });
}

int VehicleListModel::rowCount(const QModelIndex &parent) const
{
    return DataContainer::instance()->getVehiclesCount();
}

QVariant VehicleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    const auto *vehicle = DataContainer::instance()->getVehicleByIndex(index.row());
    if (nullptr == vehicle)
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        return vehicle->name() + " / " + vehicle->plateNo();
    case RoleVehicleId:
        return vehicle->id();
    }

    return QVariant();
}
