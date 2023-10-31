#ifndef EGVEHICLEDATA_H
#define EGVEHICLEDATA_H

#include "qvariant.h"
#include <QDateTime>
#include <QList>
#include <QString>

struct EgVehicleData {
  int id;
  QString name;
  QString plateNo;
  double temperature;
  double batteryVoltage;
};

enum class EgSensorDataType { Temperature1, BatteryVoltage };

struct EgSensorData {
  int vehicleId;
  EgSensorDataType dataType;
  QDateTime timestamp;
  QVariant value;
};

struct EgVehicleListData {
  QList<EgVehicleData *> vehicles;
};

#endif // EGVEHICLEDATA_H
