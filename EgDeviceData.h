#ifndef EGDEVICEDATA_H
#define EGDEVICEDATA_H

#include <QList>
#include <QString>

struct EgDeviceData {
  QString id;
  QString type;
  int user_id;
  int vehicle_id;
};

struct EgDevicesListData {
  QList<EgDeviceData *> devices;
};

#endif // EGDEVICEDATA_H
