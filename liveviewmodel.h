#ifndef LIVEVIEWMODEL_H
#define LIVEVIEWMODEL_H

#include "DataSource/EgDeviceData.h"
#include "DataSource/EgVehicleData.h"
#include "treeitem.h"

#include <QAbstractItemModel>
#include <QObject>

class LiveViewModel : public QAbstractItemModel {
  Q_OBJECT
public:
  explicit LiveViewModel(QObject *parent = 0);
  ~LiveViewModel();

  QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  QModelIndex
  index(int row, int column,
        const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
  int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
  int columnCount(const QModelIndex &parent = QModelIndex()) const
      Q_DECL_OVERRIDE;

public slots:
  void onVehiclesDataReceived(EgVehiclesData &vehiclesData);

  void onDevicesDataReceived(EgDevicesListData &devicesData);
  void onSensorsDataReceived(EgSensorsListData &sensorsData);

  void onSensorLiveDataReceived(EgVehicleSensorData &sensorData);

private:
  void setupModelData(TreeItem *parent);
  int findNode(unsigned int &hash, const QList<TreeItem *> &tList);

  TreeItem *rootItem;
  QMap<int, TreeItem *> vehicleItem;
  QMap<int, TreeItem *> deviceItem;
  QMap<QString, TreeItem *> sensorItem;
  QModelIndex findIndex(TreeItem *ptr, int column,
                        const QModelIndex &parent = QModelIndex());
};

#endif // LIVEVIEWMODEL_H
