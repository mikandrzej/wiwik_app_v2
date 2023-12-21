#ifndef EGVEHICLEDATA_H
#define EGVEHICLEDATA_H

#include <QColor>
#include <QDateTime>
#include <QList>
#include <QString>
#include "qgeopositioninfo.h"

struct EgVehicleData {
  int id;
  QString name;
  QString plateNo;
  QColor color;
};

enum class EgSensorDataType { NotDefined, Temperature, BatteryVoltage, GpsPosition };

struct EgVehicleSensorData {
  QString sensorAddress {};
  QString deviceAddress {};
  int vehicleId {0};
  EgSensorDataType dataType {EgSensorDataType::NotDefined};
  QDateTime timestamp {};
  double battery {0.0};
  double temperature {0.0};
  int satellites {0};
  QGeoPositionInfo geoPosition {};
  QString address {};
};

struct EgVehiclesData {
  QList<EgVehicleData *> vehicles;
};

struct EgSensorsData {
  QList<EgVehicleSensorData *> sensors;
};

#endif // EGVEHICLEDATA_H
