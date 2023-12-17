#include "vehicletreenode.h"

#include "../DataSource/datacontainer.h"

VehicleTreeNode::VehicleTreeNode(QObject *parent)
    : QObject{parent}
{
    m_type = NodeType::Root;
    connect(DataContainer::instance(),
            &DataContainer::vehicleDataInsertDone,
            this,
            [this](int newIndex) {
                auto *vehicle = DataContainer::instance()->getVehicleByIndex(newIndex);
                auto *newNode = new VehicleTreeNode(this, vehicle);
                this->addChild(newNode, newIndex);
            });
    for (auto *vehicle : DataContainer::instance()->getVehicleList()) {
        auto *newNode = new VehicleTreeNode(this, vehicle);
        addChild(newNode);
    }
}

VehicleTreeNode::VehicleTreeNode(VehicleTreeNode *parent, Vehicle *vehicle)
    : QObject{parent}
    , m_parent(parent)
    , m_vehicle(vehicle)
{
    m_type = NodeType::Vehicle;
    connect(vehicle, &Vehicle::deviceAdded, this, [this](Device *device, int newIndex) {
        auto *newNode = new VehicleTreeNode(this, device);
        this->addChild(newNode, newIndex);
    });
    connect(vehicle, &Vehicle::deviceRemoved, this, [this](int index) { this->removeChild(index); });
    connect(vehicle, &Vehicle::nameChanged, this, [this]() { emit dataChanged(this); });
    connect(vehicle, &Vehicle::plateNoChanged, this, [this]() { emit dataChanged(this); });
    auto placeholder = DataContainer::instance()->getDeviceList();
    for (auto *device : placeholder) {
        if (device->vehicle() == m_vehicle) {
            auto *newNode = new VehicleTreeNode(this, device);
            addChild(newNode);
        }
    }
}

VehicleTreeNode::VehicleTreeNode(VehicleTreeNode *parent, Device *device)
    : QObject{parent}
    , m_parent(parent)
    , m_device(device)
{
    m_type = NodeType::Device;

    connect(device, &Device::sensorAdded, this, [this](Sensor *sensor, int newIndex) {
        auto *newNode = new VehicleTreeNode(this, sensor);
        this->addChild(newNode, newIndex);
    });
    connect(device, &Device::sensorRemoved, this, [this](int index) { this->removeChild(index); });
    connect(device, &Device::nameChanged, this, [this]() { emit dataChanged(this); });
    connect(device, &Device::vehicleIdChanged, this, [this]() { emit dataChanged(this); });
    auto placeholder = DataContainer::instance()->getSensorsList();
    for (auto *sensor : placeholder) {
        if (sensor->device() == m_device) {
            auto *newNode = new VehicleTreeNode(this, sensor);
            addChild(newNode);
        }
    }
}

VehicleTreeNode::VehicleTreeNode(VehicleTreeNode *parent, Sensor *sensor)
    : QObject{parent}
    , m_parent(parent)
    , m_sensor(sensor)
{
    m_type = NodeType::Sensor;

    connect(sensor, &Sensor::nameChanged, this, [this]() { emit dataChanged(this); });
    connect(sensor, &Sensor::addressChanged, this, [this]() { emit dataChanged(this); });
    connect(sensor, &Sensor::lastMeasureChanged, this, [this]() { emit dataChanged(this); });
    connect(sensor, &Sensor::typeChanged, this, [this]() { emit dataChanged(this); });
}

QVariant VehicleTreeNode::data(int column, int role)
{
    switch (m_type) {
    case NodeType::Vehicle:
        return vehicleData(column, role);
        break;
    case NodeType::Device:
        return deviceData(column, role);
        break;
    case NodeType::Sensor:
        return sensorData(column, role);
        break;
    case NodeType::Root:
        return QVariant();
        break;
    }

    return QVariant();
}

int VehicleTreeNode::columnCount()
{
    return 2;
}

int VehicleTreeNode::childCount()
{
    return m_childs.count();
}

void VehicleTreeNode::addChild(VehicleTreeNode *node)
{
    emit childToBeInserted(this, m_childs.count());
    connect(node, &VehicleTreeNode::childToBeInserted, this, &VehicleTreeNode::childToBeInserted);
    connect(node, &VehicleTreeNode::childInsertDone, this, &VehicleTreeNode::childInsertDone);
    connect(node, &VehicleTreeNode::dataChanged, this, &VehicleTreeNode::dataChanged);
    m_childs.append(node);
    emit childInsertDone();
}

void VehicleTreeNode::addChild(VehicleTreeNode *node, int index)
{
    emit childToBeInserted(this, index);
    connect(node, &VehicleTreeNode::childToBeInserted, this, &VehicleTreeNode::childToBeInserted);
    connect(node, &VehicleTreeNode::childInsertDone, this, &VehicleTreeNode::childInsertDone);
    connect(node, &VehicleTreeNode::dataChanged, this, &VehicleTreeNode::dataChanged);
    m_childs.insert(index, node);
    emit childInsertDone();
}

void VehicleTreeNode::removeChild(int index)
{
    emit childToBeRemoved(this, index);
    m_childs.removeAt(index);
    emit childRemovingDone();
}

QVariant VehicleTreeNode::vehicleData(int column, int role)
{
    static auto icon = QIcon(":/icons8-car-50.png");

    switch (column) {
    case ColumnName:
        switch (role) {
        case Qt::DisplayRole:
            return m_vehicle->name().length() > 0 ? m_vehicle->name() : "Bez nazwy";
            break;
        case Qt::DecorationRole:
            return icon;
            break;
        }
        break;
    case ColumnData:
        switch (role) {
        case Qt::DisplayRole:
            return m_vehicle->plateNo();
            break;
        }
        break;
    }
    return QVariant();
}

QVariant VehicleTreeNode::deviceData(int column, int role)
{
    static auto icon = QIcon(":/icons8-device-50.png");
    switch (column) {
    case ColumnName:
        switch (role) {
        case Qt::DisplayRole:
            return m_device->name().length() > 0 ? m_device->name() : "Bez nazwy";
            break;
        case Qt::DecorationRole:
            return icon;
            break;
        }
        break;
    case ColumnData:
        switch (role) {
        case Qt::DisplayRole:
            return m_device->serialNo();
            break;
        }
        break;
    }
    return QVariant();
}

QVariant VehicleTreeNode::sensorData(int column, int role)
{
    static auto batteryIcon = QIcon(":/icons8-battery-60.png");
    static auto locationIcon = QIcon(":/icons8-gps-50.png");
    static auto temperatureIcon = QIcon(":/icons8-temperature-50.png");

    switch (column) {
    case ColumnName:
        switch (role) {
        case Qt::DisplayRole:
            return m_sensor->name().length() > 0 ? m_sensor->name() : "Bez nazwy";
            break;
        case Qt::DecorationRole:
            if (m_sensor->type() == "temperature")
                return temperatureIcon;
            else if (m_sensor->type() == "battery")
                return batteryIcon;
            else if (m_sensor->type() == "gps")
                return locationIcon;
            else
                return QVariant();
            break;
        }
        break;
    case ColumnData:
        switch (role) {
        case Qt::DisplayRole:
            return m_sensor->lastMeasureString();
        }
        break;
    }
    return QVariant();
}

QList<VehicleTreeNode *> VehicleTreeNode::childs()
{
    return m_childs;
}

VehicleTreeNode *VehicleTreeNode::parent() const
{
    return m_parent;
}
