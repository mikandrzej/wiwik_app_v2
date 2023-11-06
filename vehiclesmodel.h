#ifndef VEHICLESMODEL_H
#define VEHICLESMODEL_H

#include "EgVehicleData.h"
#include "onevehiclemodel.h"
#include <QAbstractTableModel>

class VehiclesModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit VehiclesModel(QObject *parent = nullptr);

  // QAbstractItemModel interface
public:
  QVariant data(const QModelIndex &index, int role) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  QHash<int, QByteArray> roleNames() const override;
  enum ColumnTypes {
    ColumnId,
    ColumnName,
    ColumnPlateNo,
    ColumnTemperature,
    ColumnPosition,
    ColumnBattery,
    ColumnMax
  };
  enum Roles { RoleHistoryData = Qt::UserRole };

public slots:
  void onSensorDataReceived(EgSensorData &sensorData);
  void onSensorsDataReceived(EgSensorsData &sensorsData);
  void onVehiclesDataReceived(EgVehiclesData &vehiclesData);

private:
  QList<OneVehicleModel *> m_vehicles;
  QHash<int, OneVehicleModel *> m_vehiclesById;
  QHash<OneVehicleModel *, int> m_vehicleRowByModel;
};

#endif // VEHICLESMODEL_H
