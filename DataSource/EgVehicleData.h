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

enum class EgSensorDataType { Temperature, BatteryVoltage, GpsPosition };

struct EgVehicleSensorData {
  QString sensorAddress;
  QString deviceAddress;
  int vehicleId;
  EgSensorDataType dataType;
  QDateTime timestamp;
  double battery;
  double temperature;
  int satellites;
  QGeoPositionInfo geoPosition;
  QString address;
};

struct EgVehiclesData {
  QList<EgVehicleData *> vehicles;
};

struct EgSensorsData {
  QList<EgVehicleSensorData *> sensors;
};

#endif // EGVEHICLEDATA_H
