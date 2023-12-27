#ifndef EGDEVICEDATA_H
#define EGDEVICEDATA_H

#include "../DataModels/gpsdata.h"
#include <QDateTime>
#include <QGeoLocation>
#include <QList>
#include <QMap>
#include <QString>

struct EgSensorData
{
    int id;
    QString name;
    QString address;
    QString type;
    int device_id;
    QString device_name;
};

struct EgSensorsListData
{
    QList<EgSensorData*> sensors;
};

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

struct EgGpsData
{
    QVector<GpsData> geolocations;
    QVector<QDateTime> timestamps;
    QString sensor_address;
    QString sensor_name;
};
struct EgGpsListData
{
    QMap<QString, EgGpsData*> sensors;
};

#endif   // EGDEVICEDATA_H
