#ifndef GPSDATA_H
#define GPSDATA_H

#include <QDateTime>
#include <QGeoAddress>
#include <QGeoCoordinate>
#include <QGeoLocation>
#include <QObject>

struct GpsData
{
public:
    explicit GpsData(QObject *parent = nullptr);

    QGeoCoordinate coordinate() const;
    void setCoordinate(const QGeoCoordinate &newCoordinate);

    QGeoAddress address() const;
    void setAddress(const QGeoAddress &newAddress);

    double speed() const;
    void setSpeed(double newSpeed);

    double accuracy() const;
    void setAccuracy(double newAccuracy);

    QDateTime gpsTimestamp() const;
    void setGpsTimestamp(const QDateTime &newGpsTimestamp);

signals:

    void coordinateChanged();

    void addressChanged();

    void speedChanged();

    void accuracyChanged();

    void gpsTimestampChanged();

private:
    QGeoCoordinate m_coordinate;
    QGeoAddress m_address;
    double m_speed = 0.0;
    double m_accuracy = 0.0;
    QDateTime m_gpsTimestamp;
};

Q_DECLARE_METATYPE(GpsData)

#endif // GPSDATA_H
