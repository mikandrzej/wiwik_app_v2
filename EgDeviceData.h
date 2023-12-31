#ifndef EGDEVICEDATA_H
#define EGDEVICEDATA_H

#include <QList>
#include <QMap>
#include <QString>

struct EgDeviceData
{
    int id;
    QString name;
    QString serial_no;
    QString type;
    int user_id;
    int vehicle_id;
};

struct EgDevicesListData
{
    QList<EgDeviceData*> devices;
};

struct EgTemperatureData
{
    QVector<double> timestamps;
    QVector<double> values;
    QString sensor_address;
    QString sensor_name;
};

struct EgTemperatureListData
{
    QMap<QString, EgTemperatureData*> sensors;
};

#endif   // EGDEVICEDATA_H
