#ifndef EGVEHICLEDATA_H
#define EGVEHICLEDATA_H

#include <QDateTime>
#include <QList>
#include <QString>

struct EgVehicleData {
  int id;
  QString name;
  QString plateNo;
  double temperature;
};

struct EgSensorData {
  int vehicleId;
  QDateTime timestamp;
  double temperature;
};

struct EgVehicleListData {
  QList<EgVehicleData *> vehicles;
};

#endif // EGVEHICLEDATA_H
