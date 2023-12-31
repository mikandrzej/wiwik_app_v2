#include "vehicletreenode.h"
#include "../DataSource/datacontainer.h"
#include <QIcon>
#include <QPainter>
#include <QPixmap>

VehicleTreeNode::VehicleTreeNode(QObject* parent) : QObject {parent}
{
    m_type = NodeType::Root;
    connect(DataContainer::instance(), &DataContainer::vehicleDataInsertDone, this, [this](int newIndex) {
        auto* vehicle = DataContainer::instance()->getVehicleByIndex(newIndex);
        auto* newNode = new VehicleTreeNode(this, vehicle);
        this->addChild(newNode, newIndex);
    });
    for (auto* vehicle : DataContainer::instance()->getVehicleList())
    {
        auto* newNode = new VehicleTreeNode(this, vehicle);
        addChild(newNode);
    }
}

VehicleTreeNode::VehicleTreeNode(VehicleTreeNode* parent, Vehicle* vehicle) :
    QObject {parent}

    ,
    m_vehicle(vehicle),
    m_parent(parent)
{
    m_type = NodeType::Vehicle;
    connect(vehicle, &Vehicle::deviceAdded, this, [this](Device* device, int newIndex) {
        auto* newNode = new VehicleTreeNode(this, device);
        this->addChild(newNode, newIndex);
    });
    connect(vehicle, &Vehicle::deviceRemoved, this, [this](int index) { this->removeChild(index); });
    connect(vehicle, &Vehicle::nameChanged, this, [this]() { emit dataChanged(this); });
    connect(vehicle, &Vehicle::plateNoChanged, this, [this]() { emit dataChanged(this); });
    auto placeholder = DataContainer::instance()->getDeviceList();
    for (auto* device : placeholder)
    {
        if (device->vehicle() == m_vehicle)
        {
            auto* newNode = new VehicleTreeNode(this, device);
            addChild(newNode);
        }
    }
}

VehicleTreeNode::VehicleTreeNode(VehicleTreeNode* parent, Device* device) : QObject {parent}, m_device(device), m_parent(parent)
{
    m_type = NodeType::Device;

    connect(device, &Device::sensorAdded, this, [this](Sensor* sensor, int newIndex) {
        auto* newNode = new VehicleTreeNode(this, sensor);
        this->addChild(newNode, newIndex);
    });
    connect(device, &Device::sensorRemoved, this, [this](int index) { this->removeChild(index); });
    connect(device, &Device::nameChanged, this, [this]() { emit dataChanged(this); });
    connect(device, &Device::vehicleIdChanged, this, [this]() { emit dataChanged(this); });
    auto placeholder = DataContainer::instance()->getSensorsList();
    for (auto* sensor : placeholder)
    {
        if (sensor->device() == m_device)
        {
            auto* newNode = new VehicleTreeNode(this, sensor);
            addChild(newNode);
        }
    }
}

VehicleTreeNode::VehicleTreeNode(VehicleTreeNode* parent, Sensor* sensor) :
    QObject {parent}

    ,
    m_sensor(sensor),
    m_parent(parent)
{
    m_type = NodeType::Sensor;

    connect(sensor, &Sensor::nameChanged, this, [this]() { emit dataChanged(this); });
    connect(sensor, &Sensor::addressChanged, this, [this]() { emit dataChanged(this); });
    connect(sensor, &Sensor::lastMeasureChanged, this, [this]() { emit dataChanged(this); });
    connect(sensor, &Sensor::typeChanged, this, [this]() { emit dataChanged(this); });
}

QVariant VehicleTreeNode::data(int column, int role)
{
    switch (m_type)
    {
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
            return {};
            break;
    }

    return {};
}

int VehicleTreeNode::columnCount()
{
    return 2;
}

int VehicleTreeNode::childCount()
{
    return m_childs.count();
}

void VehicleTreeNode::addChild(VehicleTreeNode* node)
{
    emit childToBeInserted(this, m_childs.count());
    connect(node, &VehicleTreeNode::childToBeInserted, this, &VehicleTreeNode::childToBeInserted);
    connect(node, &VehicleTreeNode::childInsertDone, this, &VehicleTreeNode::childInsertDone);
    connect(node, &VehicleTreeNode::dataChanged, this, &VehicleTreeNode::dataChanged);
    m_childs.append(node);
    emit childInsertDone();
}

void VehicleTreeNode::addChild(VehicleTreeNode* node, int index)
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

    switch (column)
    {
        case ColumnName:
            switch (role)
            {
                case Qt::DisplayRole:
                    return m_vehicle->name().length() > 0 ? m_vehicle->name() : "Bez nazwy";
                    break;
                case Qt::DecorationRole:
                    return TransformIconColor(m_vehicle->color(), icon);
                    break;
            }
            break;
        case ColumnData:
            switch (role)
            {
                case Qt::DisplayRole:
                    return m_vehicle->plateNo();
                    break;
            }
            break;
    }
    return {};
}

QVariant VehicleTreeNode::deviceData(int column, int role)
{
    static auto icon = QIcon(":/icons8-device-50.png");
    switch (column)
    {
        case ColumnName:
            switch (role)
            {
                case Qt::DisplayRole:
                    return m_device->name().length() > 0 ? m_device->name() : "Bez nazwy";
                    break;
                case Qt::DecorationRole:
                    return icon;
                    break;
            }
            break;
        case ColumnData:
            switch (role)
            {
                case Qt::DisplayRole:
                    return m_device->serialNo();
                    break;
            }
            break;
    }
    return {};
}

QVariant VehicleTreeNode::sensorData(int column, int role)
{
    static auto batteryIcon = QIcon(":/icons8-battery-60.png");
    static auto locationIcon = QIcon(":/icons8-gps-50.png");
    static auto temperatureIcon = QIcon(":/icons8-temperature-50.png");

    switch (column)
    {
        case ColumnName:
            switch (role)
            {
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
                        return {};
                    break;
            }
            break;
        case ColumnData:
            switch (role)
            {
                case Qt::DisplayRole:
                    return m_sensor->lastMeasureString();
            }
            break;
    }
    return {};
}

QList<VehicleTreeNode*> VehicleTreeNode::childs()
{
    return m_childs;
}

VehicleTreeNode* VehicleTreeNode::parent() const
{
    return m_parent;
}

QIcon VehicleTreeNode::TransformIconColor(const QColor& newColor, const QIcon& originalIcon)
{
    // Get the original pixmap from the original icon
    QPixmap originalPixmap = originalIcon.pixmap(originalIcon.availableSizes().first());

    // Convert the original pixmap to a QImage for direct manipulation
    QImage originalImage = originalPixmap.toImage();

    // Iterate through each pixel and change black to the specified color
    for (int x = 0; x < originalImage.width(); ++x)
    {
        for (int y = 0; y < originalImage.height(); ++y)
        {
            QRgb pixelColor = originalImage.pixel(x, y);

            // Check if the pixel is black (you may need to adjust the tolerance)
            if (qRed(pixelColor) < 50 && qGreen(pixelColor) < 50 && qBlue(pixelColor) < 50)
            {
                // Change black to the specified color while keeping the alpha channel
                originalImage.setPixelColor(x, y, QColor(newColor.red(), newColor.green(), newColor.blue(), qAlpha(pixelColor)));
            }
        }
    }

    // Create a new QIcon from the modified QImage
    QIcon newIcon(QPixmap::fromImage(originalImage));

    return newIcon;
}
