#include "vehicletablemodel.h"
#include "../DataModels/vehicle.h"
#include "../DataSource/datacontainer.h"
#include "QtGui/qcolor.h"

VehicleTableModel vehicleTableModel;

VehicleTableModel::VehicleTableModel(QObject* parent) : QAbstractTableModel {parent}
{
    connect(DataContainer::instance(), &DataContainer::vehicleDataToBeInserted, this, [this](int row) { beginInsertRows(QModelIndex(), row, row); });
    connect(DataContainer::instance(), &DataContainer::vehicleDataInsertDone, this, [this]() { endInsertRows(); });
    connect(DataContainer::instance(), &DataContainer::vehicleDataChanged, this, [this](int idx) {
        emit dataChanged(createIndex(idx, 0), createIndex(idx, Columns::ColumnMax - 1), {Qt::DisplayRole, Qt::CheckStateRole, Qt::BackgroundRole});
    });
}

int VehicleTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return DataContainer::instance()->getVehiclesCount();
}

int VehicleTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(ColumnMax);
}

QVariant VehicleTableModel::data(const QModelIndex& index, int role) const
{
    const auto* vehicle = DataContainer::instance()->getVehicleByIndex(index.row());
    if (nullptr == vehicle)
        return QVariant();

    const auto column = index.column();
    if (Qt::DisplayRole == role || Qt::EditRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnId:
                return vehicle->id();
            case ColumnName:
                return vehicle->name();
            case ColumnPlateNo:
                return vehicle->plateNo();
            case ColumnColor:
                return vehicle->color();
            default:
                break;
        }
    }
    else if (Qt::BackgroundRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnCommit:
                if (vehicle->changesPending())
                {
                    return QColorConstants::Magenta;
                }
                break;
            default:
                break;
        }
    }
    else if (Qt::CheckStateRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnCommit:
                if (vehicle->changesPending())
                    return !vehicle->changesPending();
            default:
                break;
        }
    }
    return QVariant();
}

bool VehicleTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto* vehicle = DataContainer::instance()->getVehicleByIndex(index.row());
    if (nullptr == vehicle)
        return false;

    const auto column = index.column();
    if (Qt::EditRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnName:
                vehicle->setName(value.toString());
                return true;
            case ColumnPlateNo:
                vehicle->setPlateNo(value.toString());
                return true;
            case ColumnColor:
                vehicle->setColor(value.value<QColor>());
                return true;
            default:
                break;
        }
    }
    else if (Qt::CheckStateRole == role)
    {
        if (ColumnCommit == static_cast<Columns>(column))
        {
            vehicle->commitChanges();
        }
    }

    return false;
}

QVariant VehicleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (m_columnData.contains(section))
    {
        if (Qt::Orientation::Horizontal == orientation)
        {
            if (Qt::DisplayRole == role)
            {
                return m_columnData[section].name;
            }
        }
    }

    return QVariant();
}

Qt::ItemFlags VehicleTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable;

    const auto* vehicle = DataContainer::instance()->getVehicleByIndex(index.row());
    switch (static_cast<Columns>(index.column()))
    {
        case ColumnName:
        case ColumnPlateNo:
        case ColumnColor:
            flags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;
            break;
        case ColumnCommit:
            if (vehicle)
                if (vehicle->changesPending())
                    flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
            break;
        default:
            break;
    }

    return flags;
}

void VehicleTableModel::onDataReset()
{
    beginResetModel();
    endResetModel();
}
