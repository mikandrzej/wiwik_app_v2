#include "vehicle.h"

#include "../Configuration/configuration.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

Vehicle::Vehicle(int id, QString &name, QString &plateNo, QObject *parent)
    : m_id(id), m_name(name), m_plateNo(plateNo), QObject(parent) {}

Vehicle::Vehicle(const Vehicle &vehicle, QObject *parent)
    : m_id(vehicle.id()), m_name(vehicle.name()), m_plateNo(vehicle.plateNo()),
      QObject(parent) {}

int Vehicle::id() const { return m_id; }

QString Vehicle::name() const { return m_name; }

void Vehicle::setName(const QString &newName) {
  if (m_name == newName)
    return;
  m_name = newName;
  setChangesPending(true);
  emit nameChanged();
}

QString Vehicle::plateNo() const { return m_plateNo; }

void Vehicle::setPlateNo(const QString &newPlateNo) {
  if (m_plateNo == newPlateNo)
    return;
  m_plateNo = newPlateNo;
  setChangesPending(true);
  emit plateNoChanged();
}

void Vehicle::commitChanges() {
  if (!m_changesPending)
    return;

  QString serverUrl = configuration.getConfiguration("server_url").toString();
  serverUrl += "/api/setVehicleData";

  QUrl url(serverUrl);
  QUrlQuery uq;

  uq.addQueryItem("vehicle_id", QString::number(m_id));
  uq.addQueryItem("name", m_name);
  uq.addQueryItem("plate", m_plateNo);

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
        qWarning() << "commit changes for vehicle " << m_id
                   << "error with code: " << status_code;
      }
    }
  });
}

bool Vehicle::changesPending() const { return m_changesPending; }

void Vehicle::setChangesPending(bool newChangesPending) {
  if (m_changesPending == newChangesPending)
    return;
  m_changesPending = newChangesPending;
  emit changesPendingChanged();
}

void Vehicle::addDevice(Device *device)
{
    m_devices.append(device);
    emit deviceAdded(device, m_devices.indexOf(device));
}

void Vehicle::removeDevice(Device *device)
{
    int removedIndex = m_devices.indexOf(device);
    m_devices.removeOne(device);
    emit deviceRemoved(removedIndex);
}

QList<Device *> Vehicle::devices() const
{
    return m_devices;
}
