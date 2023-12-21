#ifndef VEHMODELTREEITEM_H
#define VEHMODELTREEITEM_H

#include <QObject>
#include <QVariant>

class MainVehicleModelData : public QObject {
  Q_OBJECT
public:
  MainVehicleModelData(const MainVehicleModelData *sourceObject = nullptr,
                       QObject *parent = nullptr);

  int id() const;
  void setId(int newId);
  QString name() const;
  void setName(const QString &newName);
  QString plateNo() const;
  void setPlateNo(const QString &newPlateNo);
  bool active() const;
  void setActive(bool newActive);

signals:
  void idChanged();
  void nameChanged();
  void plateNoChanged();
  void activeChanged();

private:
  int m_id;
  QString m_name;
  QString m_plateNo;
  bool m_active;
};

class MainDeviceModelData : public QObject {
  Q_OBJECT
public:
  MainDeviceModelData(const MainDeviceModelData *sourceObject = nullptr,
                      QObject *parent = nullptr);

  int id() const;
  void setId(int newId);
  QString serialNo() const;
  void setSerialNo(const QString &newSerialNo);
  QString name() const;
  void setName(const QString &newName);
  bool active() const;
  void setActive(bool newActive);
  int vehicleId() const;
  void setVehicleId(int newVehicleId);
  int typeId() const;
  void setTypeId(int newTypeId);

signals:
  void idChanged();
  void serialNoChanged();
  void nameChanged();
  void activeChanged();
  void vehicleIdChanged();
  void typeIdChanged();

private:
  int m_id;
  QString m_serialNo;
  QString m_name;
  bool m_active;
  int m_vehicleId;
  int m_typeId;
};

enum class TreeItemType { Root, Vehicle, Device, Sensor };

struct MainModelData {
  TreeItemType itemType;
  MainVehicleModelData *vehicleData {nullptr};
  MainDeviceModelData *deviceData {nullptr};
};

class VehModelTreeItem {
public:
  explicit VehModelTreeItem(const MainModelData &data,
                            VehModelTreeItem *parentItem = nullptr);
  ~VehModelTreeItem();

  void appendChild(VehModelTreeItem *child);

  VehModelTreeItem *child(int row);
  int childCount() const;
  int row() const;
  VehModelTreeItem *parentItem();

  const MainModelData &data() const;

private:
  QList<VehModelTreeItem *> m_childItems;
  MainModelData m_itemData;
  VehModelTreeItem *m_parentItem;
};

#endif // VEHMODELTREEITEM_H
