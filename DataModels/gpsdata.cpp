#include "gpsdata.h"

QGeoCoordinate GpsData::coordinate() const
{
    return m_coordinate;
}

void GpsData::setCoordinate(const QGeoCoordinate &newCoordinate)
{
    if (m_coordinate == newCoordinate)
        return;
    m_coordinate = newCoordinate;
}

QGeoAddress GpsData::address() const
{
    return m_address;
}

void GpsData::setAddress(const QGeoAddress &newAddress)
{
    if (m_address == newAddress)
        return;
    m_address = newAddress;
}

double GpsData::speed() const
{
    return m_speed;
}

void GpsData::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(m_speed, newSpeed))
        return;
    m_speed = newSpeed;
}

double GpsData::accuracy() const
{
    return m_accuracy;
}

void GpsData::setAccuracy(double newAccuracy)
{
    if (qFuzzyCompare(m_accuracy, newAccuracy))
        return;
    m_accuracy = newAccuracy;
}

QDateTime GpsData::gpsTimestamp() const
{
    return m_gpsTimestamp;
}

void GpsData::setGpsTimestamp(const QDateTime &newGpsTimestamp)
{
    if (m_gpsTimestamp == newGpsTimestamp)
        return;
    m_gpsTimestamp = newGpsTimestamp;
}
