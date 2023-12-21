#ifndef VEHICLETREENODE_H
#define VEHICLETREENODE_H

#include "../DataModels/device.h"
#include "../DataModels/sensor.h"
#include "../DataModels/vehicle.h"
#include <QIcon>

class VehicleTreeNode : public QObject
{
    Q_OBJECT
  public:
    VehicleTreeNode(QObject* parent = nullptr);
    VehicleTreeNode(VehicleTreeNode* parent, Vehicle* vehicle);
    VehicleTreeNode(VehicleTreeNode* parent, Device* device);
    VehicleTreeNode(VehicleTreeNode* parent, Sensor* sensor);
    QVariant data(int column, int role);
    int columnCount();
    int childCount();
    void addChild(VehicleTreeNode* node);
    void addChild(VehicleTreeNode* node, int index);

    VehicleTreeNode* parent() const;

    QList<VehicleTreeNode*> childs();

  signals:
    void childToBeInserted(VehicleTreeNode* node, int index);
    void childInsertDone();
    void childToBeRemoved(VehicleTreeNode* node, int index);
    void childRemovingDone();
    void dataChanged(VehicleTreeNode* node);

  private slots:
    void removeChild(int index);

  private:
    enum class NodeType
    {
        Root,
        Vehicle,
        Device,
        Sensor
    } m_type;

    enum Columns
    {
        ColumnName,
        ColumnData
    };

    Vehicle* m_vehicle;
    Device* m_device;
    Sensor* m_sensor;

    QVariant vehicleData(int column, int role);
    QVariant deviceData(int column, int role);
    QVariant sensorData(int column, int role);

    VehicleTreeNode* m_parent = nullptr;
    QList<VehicleTreeNode*> m_childs;
    QIcon TransformIconColor(QColor newColor, QIcon originalIcon);
};

#endif   // VEHICLETREENODE_H
