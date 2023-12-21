#include "sensor.h"
#include <QGeoPositionInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include "../Configuration/configuration.h"
#include "gpsdata.h"

Sensor::Sensor(int id, const QString &name, const QString &address,
               const QString &type, int deviceId, QObject *parent)
    : QObject(parent), m_id(id), m_name(name), m_address(address), m_type(type),
      m_deviceId(deviceId) {}

Sensor::Sensor(const Sensor &sensor, QObject *parent)
    : m_id(sensor.id()), m_name(sensor.name()), m_address(sensor.address()),
    m_type(sensor.type()), m_deviceId(sensor.deviceId()), QObject{parent} {}

Sensor::Sensor(const QString &address, QObject *parent) : m_address(address), QObject{parent}
{

}

QString Sensor::name() const { return m_name; }

void Sensor::setName(const QString &newName) {
  if (m_name == newName)
    return;
  m_name = newName;
  setChangesPending(true);
  emit nameChanged();
}

bool Sensor::changesPending() const { return m_changesPending; }

void Sensor::setChangesPending(bool newChangesPending) {
  if (m_changesPending == newChangesPending)
    return;
  m_changesPending = newChangesPending;
  emit changesPendingChanged();
}

void Sensor::commitChanges() {
  if (!m_changesPending)
    return;

  QString serverUrl = configuration.getConfiguration("server_url").toString();
  serverUrl += "/api/setSensorData";

  QUrl url(serverUrl);
  QUrlQuery uq;

  uq.addQueryItem("sensor_id", QString::number(m_id));
  uq.addQueryItem("name", m_name);

  url.setQuery(uq);
  QNetworkRequest request(url);

  if (nullptr == m_netAccMgr)
    m_netAccMgr = new QNetworkAccessManager(this);

  QNetworkReply *reply = m_netAccMgr->get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    QVariant status_code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) {
      if (status_code == 200) {
        setChangesPending(false);
      } else {
        qWarning() << "commit changes for sensor " << m_id
                   << "error with code: " << status_code;
      }
    }
  });
}

Measure *Sensor::lastMeasure() const { return m_lastMeasure; }

QString Sensor::lastMeasureString() const
{
    if(m_lastMeasure != nullptr)
    {
        if(m_type == "temperature")
        {
            return m_lastMeasure->value().toString() + "'C";
        }
        else if(m_type == "battery")
        {
            return QString::number(m_lastMeasure->value().toDouble(), 'f', 0) + "mV";
        }
        else if(m_type == "gps")
        {
            auto gpsData = m_lastMeasure->value().value<GpsData>();
            return gpsData.address().text();
        }
    }
    return "";
}

void Sensor::addMeasure(Measure &newMeasure) {
  if (m_measures.contains(newMeasure.timestamp()))
    return;

  auto *newMasurePtr = new Measure(&newMeasure);

  m_measures.insert(newMasurePtr->timestamp(), newMasurePtr);

  bool newest = false;
  if (nullptr == m_lastMeasure) {
    newest = true;
  } else if (newMasurePtr->timestamp() > m_lastMeasure->timestamp()) {
    newest = true;
  }

  if (newest) {
    m_lastMeasure = newMasurePtr;
    emit lastMeasureChanged();
  }

  emit measuresChanged();
}

QString Sensor::type() const
{
    return m_type;
}

void Sensor::setType(const QString &newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

QString Sensor::address() const
{
    return m_address;
}

void Sensor::setAddress(const QString &newAddress)
{
    if (m_address == newAddress)
        return;
    m_address = newAddress;
    emit addressChanged();
}

int Sensor::id() const
{
    return m_id;
}

void Sensor::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

Device *Sensor::device() const
{
    return m_device;
}

void Sensor::setDevice(Device *newDevice)
{
    if (m_device == newDevice)
        return;
    m_device = newDevice;
    emit deviceChanged();
}

int Sensor::deviceId() const
{
    return m_deviceId;
}

void Sensor::setDeviceId(int newDeviceId)
{
    if (m_deviceId == newDeviceId)
        return;
    m_deviceId = newDeviceId;
    emit deviceIdChanged();
}

QMap<QDateTime, Measure *> Sensor::measures() const { return m_measures; }

void Sensor::setMeasures(const QMap<QDateTime, Measure *> &newMeasures) {
  if (m_measures == newMeasures)
    return;
  m_measures = newMeasures;
  emit measuresChanged();
}
