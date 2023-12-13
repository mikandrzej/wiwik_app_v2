#ifndef MAINVEHICLEMODEL_H
#define MAINVEHICLEMODEL_H

#include "vehmodeltreeitem.h"

#include <QAbstractItemModel>
#include <QObject>

class VehModelTreeItem;

class MainVehicleModel : public QAbstractItemModel {
  Q_OBJECT
public:
  explicit MainVehicleModel(QObject *parent = nullptr);

  enum VehicleColumns {
    VehicleColumnId,
    VehicleColumnName,
    VehicleColumnPlateNo,
    VehicleColumnActive,
    VehicleColumnMax,
  };

  enum DeviceColumns {
    DeviceColumnId,
    DeviceColumnSerial,
    DeviceColumnName,
    DeviceColumnActive,
    DeviceColumnVehicleId,
    DeviceColumnTypeId,
    DeviceColumnMax,
  };

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  void extracted(const QModelIndex &index) const;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;

public slots:
  void onNewVehicleData(const MainVehicleModelData &vehicleData);
  void onNewDeviceData(const MainDeviceModelData &deviceData);

private:
  QList<MainVehicleModelData *> m_vehiclesData;
  QMap<int, MainVehicleModelData *> m_vehiclesDataById;
  QList<MainDeviceModelData *> m_devicesData;
  QMap<int, MainDeviceModelData *> m_devicesDataById;

  VehModelTreeItem *rootItem;
};

#endif // MAINVEHICLEMODEL_H
