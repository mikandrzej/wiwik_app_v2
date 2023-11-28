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

struct EgSensorData {
  QString sensorAddress;
  QString deviceAddress;
  int vehicleId;
  EgSensorDataType dataType;
  QDateTime timestamp;
  union {
    double battery;
    double temperature;
  };
  QGeoPositionInfo geoPosition;
};

struct EgVehiclesData {
  QList<EgVehicleData *> vehicles;
};

struct EgSensorsData {
  QList<EgSensorData *> sensors;
};

#endif // EGVEHICLEDATA_H
