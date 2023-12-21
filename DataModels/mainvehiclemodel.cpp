#include "mainvehiclemodel.h"

MainVehicleModel::MainVehicleModel(QObject* parent) : QAbstractItemModel {parent}
{
    MainModelData mdata = {.itemType = TreeItemType::Root};
    rootItem = new VehModelTreeItem(mdata);
}

QModelIndex MainVehicleModel::index(int row, int column, const QModelIndex& /*parent*/) const
{
    return createIndex(row, column);
}

QModelIndex MainVehicleModel::parent(const QModelIndex& /*child*/) const
{
    return {};
}

int MainVehicleModel::rowCount(const QModelIndex& parent) const
{
    VehModelTreeItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<VehModelTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int MainVehicleModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        auto* item = static_cast<VehModelTreeItem*>(parent.internalPointer());
        switch (item->data().itemType)
        {
            case TreeItemType::Vehicle:
            case TreeItemType::Device:
            case TreeItemType::Sensor:
                return 0;
                break;
            case TreeItemType::Root:
                return static_cast<int>(VehicleColumnMax);
                break;
        }
    }
    else
    {
        return static_cast<int>(VehicleColumnMax);
    }
    return 0;
}

QVariant MainVehicleModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    if (role != Qt::DisplayRole)
        return {};

    auto* item = static_cast<VehModelTreeItem*>(index.internalPointer());

    switch (item->data().itemType)
    {
        case TreeItemType::Vehicle:
            switch (static_cast<VehicleColumns>(index.column()))
            {
                case VehicleColumnId:
                    return item->data().vehicleData->id();
                case VehicleColumnName:
                    return item->data().vehicleData->name();
                case VehicleColumnPlateNo:
                    return item->data().vehicleData->plateNo();
                case VehicleColumnActive:
                    return item->data().vehicleData->active();
                case VehicleColumnMax:
                default:
                    return {};
            }

        case TreeItemType::Device:
        case TreeItemType::Sensor:
        case TreeItemType::Root:
            break;
    }

    return {};
}

void MainVehicleModel::onNewVehicleData(const MainVehicleModelData& vehicleData)
{
    if (m_vehiclesDataById.contains(vehicleData.id()))
    {
        auto data = m_vehiclesDataById[vehicleData.id()];

        data->setName(vehicleData.name());
        data->setPlateNo(vehicleData.plateNo());
        data->setActive(vehicleData.active());
    }
    else
    {
        /* new data */
        auto data = new MainVehicleModelData(&vehicleData, this);
        int newIndex = m_vehiclesData.length();
        beginInsertRows(QModelIndex(), newIndex, newIndex);
        m_vehiclesData.append(data);
        m_vehiclesDataById.insert(data->id(), data);

        connect(data, &MainVehicleModelData::nameChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::VehicleColumnName, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });
        connect(data, &MainVehicleModelData::plateNoChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::VehicleColumnPlateNo, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });
        connect(data, &MainVehicleModelData::activeChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::VehicleColumnActive, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });

        endInsertRows();
    }
}

void MainVehicleModel::onNewDeviceData(const MainDeviceModelData& deviceData)
{
    if (m_devicesDataById.contains(deviceData.id()))
    {
        auto data = m_devicesDataById[deviceData.id()];

        data->setName(deviceData.name());
        data->setSerialNo(deviceData.serialNo());
        data->setActive(deviceData.active());
        data->setVehicleId(deviceData.vehicleId());
        data->setTypeId(deviceData.typeId());
    }
    else
    {
        /* new data */
        auto data = new MainDeviceModelData(&deviceData, this);
        int newIndex = m_devicesData.length();
        beginInsertRows(QModelIndex(), newIndex, newIndex);
        m_devicesData.append(data);
        m_devicesDataById.insert(data->id(), data);

        connect(data, &MainDeviceModelData::nameChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::DeviceColumnName, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });
        connect(data, &MainDeviceModelData::serialNoChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::DeviceColumnSerial, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });
        connect(data, &MainDeviceModelData::activeChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::DeviceColumnActive, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });
        connect(data, &MainDeviceModelData::vehicleIdChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::DeviceColumnVehicleId, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });
        connect(data, &MainDeviceModelData::typeIdChanged, this, [this, newIndex]() {
            auto index = this->index(newIndex, MainVehicleModel::DeviceColumnTypeId, QModelIndex());
            emit this->dataChanged(index, index, {Qt::DisplayRole});
        });

        endInsertRows();
    }
}

MainVehicleModelData::MainVehicleModelData(const MainVehicleModelData* sourceObject, QObject* parent) : QObject(parent)
{
    if (sourceObject != nullptr)
    {
        m_id = sourceObject->id();
        m_name = sourceObject->name();
        m_plateNo = sourceObject->plateNo();
        m_active = sourceObject->active();
    }
}

int MainVehicleModelData::id() const
{
    return m_id;
}

void MainVehicleModelData::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString MainVehicleModelData::name() const
{
    return m_name;
}

void MainVehicleModelData::setName(const QString& newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QString MainVehicleModelData::plateNo() const
{
    return m_plateNo;
}

void MainVehicleModelData::setPlateNo(const QString& newPlateNo)
{
    if (m_plateNo == newPlateNo)
        return;
    m_plateNo = newPlateNo;
    emit plateNoChanged();
}

bool MainVehicleModelData::active() const
{
    return m_active;
}

void MainVehicleModelData::setActive(bool newActive)
{
    if (m_active == newActive)
        return;
    m_active = newActive;
    emit activeChanged();
}

QVariant MainVehicleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::Horizontal == orientation)
    {
        if (Qt::DisplayRole == role)
        {
            switch (static_cast<VehicleColumns>(section))
            {
                case VehicleColumnId:
                    return "Id";
                case VehicleColumnName:
                    return "Nazwa";
                case VehicleColumnPlateNo:
                    return "Numer rej.";
                case VehicleColumnActive:
                    return "Aktywny";
                default:
                case VehicleColumnMax:
                    return {};
            }
        }
    }
    return {};
}

bool MainVehicleModel::setData(const QModelIndex& /*index*/, const QVariant& /*value*/, int /*role*/)
{
    return true;
}

MainDeviceModelData::MainDeviceModelData(const MainDeviceModelData* sourceObject, QObject* /*parent*/)
{
    if (sourceObject != nullptr)
    {
        m_id = sourceObject->id();
        m_serialNo = sourceObject->serialNo();
        m_name = sourceObject->name();
        m_active = sourceObject->active();
        m_typeId = sourceObject->typeId();
        m_vehicleId = sourceObject->vehicleId();
    }
}

int MainDeviceModelData::id() const
{
    return m_id;
}

void MainDeviceModelData::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString MainDeviceModelData::serialNo() const
{
    return m_serialNo;
}

void MainDeviceModelData::setSerialNo(const QString& newSerialNo)
{
    if (m_serialNo == newSerialNo)
        return;
    m_serialNo = newSerialNo;
    emit serialNoChanged();
}

QString MainDeviceModelData::name() const
{
    return m_name;
}

void MainDeviceModelData::setName(const QString& newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

bool MainDeviceModelData::active() const
{
    return m_active;
}

void MainDeviceModelData::setActive(bool newActive)
{
    if (m_active == newActive)
        return;
    m_active = newActive;
    emit activeChanged();
}

int MainDeviceModelData::vehicleId() const
{
    return m_vehicleId;
}

void MainDeviceModelData::setVehicleId(int newVehicleId)
{
    if (m_vehicleId == newVehicleId)
        return;
    m_vehicleId = newVehicleId;
    emit vehicleIdChanged();
}

int MainDeviceModelData::typeId() const
{
    return m_typeId;
}

void MainDeviceModelData::setTypeId(int newTypeId)
{
    if (m_typeId == newTypeId)
        return;
    m_typeId = newTypeId;
    emit typeIdChanged();
}
