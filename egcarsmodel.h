#ifndef QCARSMODEL_H
#define QCARSMODEL_H

#include "EgVehicleData.h"
#include <QAbstractTableModel>
#include <QObject>

enum EgCarModelColumns : int {
  Name = 0,
  PlateNo,
  Temperature,
  BatteryVoltage,
  Length
};

class EgCarsModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit EgCarsModel(QObject *parent = nullptr);

  // QAbstractItemModel interface
public:
  QVariant data(const QModelIndex &index, int role) const override;
  void appendCar(EgVehicleData *car);
  void overrideCars(EgVehicleListData *list);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  void overrideCars(EgVehicleListData &list);

public slots:
  void onSensorDataReceived(EgSensorData &sensorData);

public:
  const EgVehicleListData &getData();

private:
  EgVehicleListData m_data;

  // QAbstractItemModel interface
public:
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
};

#endif // QCARSMODEL_H
