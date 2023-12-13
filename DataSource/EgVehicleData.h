#ifndef EGVEHICLEDATA_H
#define EGVEHICLEDATA_H

#include "qgeopositioninfo.h"
#include <QDateTime>
#include <QList>
#include <QString>

struct EgVehicleData {
  int id;
  QString name;
  QString plateNo;
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
