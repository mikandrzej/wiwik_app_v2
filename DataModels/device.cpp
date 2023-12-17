#include "device.h"
#include "../Configuration/configuration.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

Device::Device(int id, QString &serialNo, QString &name, int vehicleId,
               QString &type, QObject *parent)

    : m_id(id), m_serialNo(serialNo), m_name(name), m_vehicleId(vehicleId),
      m_type(type) {}

Device::Device(const Device &device, QObject *parent)
    : m_id(device.id()), m_serialNo(device.serialNo()), m_name(device.name()),
      m_vehicleId(device.vehicleId()), m_type(device.type()), QObject{parent} {}

int Device::id() const { return m_id; }

QString Device::serialNo() const { return m_serialNo; }

QString Device::name() const { return m_name; }

void Device::setName(const QString &newName) {
  if (m_name == newName)
    return;
  m_name = newName;
  setChangesPending(true);
  emit nameChanged();
}

int Device::vehicleId() const { return m_vehicleId; }

void Device::setVehicleId(int newVehicleId) {
  if (m_vehicleId == newVehicleId)
    return;
  m_vehicleId = newVehicleId;
  setChangesPending(true);
  emit vehicleIdChanged();
}

QString Device::type() const { return m_type; }

bool Device::changesPending() const { return m_changesPending; }

void Device::setChangesPending(bool newChangesPending) {
  if (m_changesPending == newChangesPending)
    return;
  m_changesPending = newChangesPending;
  emit changesPendingChanged();
}

void Device::addSensor(Sensor *sensor)
{
    m_sensors.append(sensor);
    emit sensorAdded(sensor, m_sensors.indexOf(sensor));
}

void Device::removeSensor(Sensor *sensor)
{
    auto index = m_sensors.indexOf(sensor);
    m_sensors.removeOne(sensor);
    emit sensorRemoved(index);
}

Vehicle *Device::vehicle() const
{
    return m_vehicle;
}

void Device::setVehicle(Vehicle *newVehicle)
{
    if (m_vehicle == newVehicle)
        return;
    m_vehicle = newVehicle;
    emit vehicleChanged();
}

void Device::commitChanges() {
  if (!m_changesPending)
    return;

  QString serverUrl = configuration.getConfiguration("server_url").toString();
  serverUrl += "/api/setDeviceData";

  QUrl url(serverUrl);
  QUrlQuery uq;

  uq.addQueryItem("device_id", QString::number(m_id));
  uq.addQueryItem("name", m_name);
  uq.addQueryItem("vehicle_id", QString::number(m_vehicleId));

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
        qWarning() << "commit changes for device " << m_id
                   << "error with code: " << status_code;
      }
    }
  });
}
