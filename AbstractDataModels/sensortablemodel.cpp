#include "sensortablemodel.h"
#include "../DataModels/device.h"
#include "../DataModels/gpsdata.h"
#include "../DataModels/sensor.h"
#include "../DataSource/datacontainer.h"
#include "QtGui/qcolor.h"
#include "qgeopositioninfo.h"

SensorTableModel sensorTableModel;

SensorTableModel::SensorTableModel(QObject* parent) : QAbstractTableModel {parent}
{
    connect(DataContainer::instance(), &DataContainer::sensorDataToBeInserted, this, [this](int row) { beginInsertRows(QModelIndex(), row, row); });
    connect(DataContainer::instance(), &DataContainer::sensorDataInsertDone, this, [this]() { endInsertRows(); });
    connect(DataContainer::instance(), &DataContainer::sensorDataChanged, this, [this](int idx) {
        emit dataChanged(createIndex(idx, 0), createIndex(idx, Columns::ColumnMax - 1), {Qt::DisplayRole, Qt::CheckStateRole, Qt::BackgroundRole});
    });
}

int SensorTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return DataContainer::instance()->getSensorsCount();
}

int SensorTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(ColumnMax);
}

QVariant SensorTableModel::data(const QModelIndex& index, int role) const
{
    const auto* sensor = DataContainer::instance()->getSensorByIndex(index.row());
    if (nullptr == sensor)
        return {};

    const auto column = index.column();
    if (Qt::DisplayRole == role || Qt::EditRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnId:
                return sensor->id();
            case ColumnName:
                return sensor->name();
            case ColumnAddress:
                return sensor->address();
            case ColumnDevice:
            {
                const auto* device = DataContainer::instance()->getDeviceById(sensor->deviceId());
                if (device != nullptr)
                    return QString::number(device->id()) + ": " + device->name();
                break;
            }
            case ColumnType:
            {
                const QMap<QString, QString> typeMap = {{"temperature", "temperatura"}, {"battery", "bateria"}};
                QString val = sensor->type();
                if (typeMap.contains(val))
                    val = typeMap[val];
                return val;
            }
            case ColumnLastMeasure:
            {
                auto meas = sensor->lastMeasure();
                if (meas != nullptr)
                {
                    auto val = meas->value();

                    switch (val.type())
                    {
                        case QVariant::Double:
                            return QString::number(val.toDouble(), 'f', 2);
                        case QVariant::UserType:
                        {
                            //   auto userType = val.userType();
                            auto tname = QString(val.typeName());
                            if (tname == "GpsData")
                            {
                                auto gpsData = val.value<GpsData>();
                                return gpsData.address().text();
                            }
                            return val.toString();
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            default:
                break;
        }
    }
    else if (Qt::BackgroundRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnCommit:
                if (sensor->changesPending())
                {
                    return QColorConstants::Magenta;
                }
            default:
                break;
        }
    }
    else if (Qt::CheckStateRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnCommit:
                if (sensor->changesPending())
                    return !sensor->changesPending();
            default:
                break;
        }
    }
    return {};
}

bool SensorTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto* sensor = DataContainer::instance()->getSensorByIndex(index.row());
    if (nullptr == sensor)
        return false;

    const auto column = index.column();
    if (Qt::EditRole == role)
    {
        switch (static_cast<Columns>(column))
        {
            case ColumnName:
                sensor->setName(value.toString());
                return true;
            default:
                break;
        }
    }
    else if (Qt::CheckStateRole == role)
    {
        if (ColumnCommit == static_cast<Columns>(column))
        {
            sensor->commitChanges();
        }
    }

    return false;
}

QVariant SensorTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

    return {};
}

Qt::ItemFlags SensorTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable;

    const auto* sensor = DataContainer::instance()->getSensorByIndex(index.row());
    switch (static_cast<Columns>(index.column()))
    {
        case ColumnName:
            flags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;
            break;
        case ColumnCommit:
            if (sensor)
                if (sensor->changesPending())
                    flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
            break;
        default:
            break;
    }

    return flags;
}

void SensorTableModel::onDataReset()
{
    beginResetModel();
    endResetModel();
}
