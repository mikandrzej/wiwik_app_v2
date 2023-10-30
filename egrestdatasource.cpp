#include "egrestdatasource.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

#include <QDebug>

EgRestDataSource::EgRestDataSource(QObject *parent) : QObject{parent} {
  m_serverStateTimeoutTimer = new QTimer(this);
  m_serverStateTimeoutTimer->setSingleShot(true);
  connect(m_serverStateTimeoutTimer, &QTimer::timeout, this,
          &EgRestDataSource::serverTimeout);

  m_serverStateTimer = new QTimer(this);
  connect(m_serverStateTimer, &QTimer::timeout, this,
          &EgRestDataSource::serverStateCheck);
  m_serverStateTimer->start(m_serverStatePeriod);
}

void EgRestDataSource::getVehicleList() {
  QUrl url(m_serverPath + m_getVehiclesSubpath);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr.get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto data = reply->readAll();
    this->onVehicleListRequestReady(data);
  });
}

void EgRestDataSource::getDevicesList() {
  QUrl url(m_serverPath + m_getDevicesSubpath);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr.get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto data = reply->readAll();
    this->onDevicesListRequestReady(data);
  });
}

void EgRestDataSource::requestVehicleHistoryData(int vehicleId, QDate &date) {

  QUrl url(m_serverPath + m_getVehicleHistoryDataPath);
  QUrlQuery query;
  query.addQueryItem("date",
                     QString::number(date.startOfDay().toSecsSinceEpoch()));
  query.addQueryItem("vehicle_id", QString::number(vehicleId));
  url.setQuery(query);

  QNetworkRequest request(url);

  QNetworkReply *reply = m_netAccMgr.get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto replyData = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData);

    QVector<int> x;
    QVector<double> y;

    if (!jsonDoc.isNull()) {
      if (jsonDoc.isArray()) {
        auto pointArray = jsonDoc.array();

        x.reserve(pointArray.count());
        y.reserve(pointArray.count());

        foreach (auto point, pointArray) {
          if (!point["measure_timestamp"].isDouble()) {
            qWarning() << "Not found timestamp key";
            return;
          }
          auto timestamp = point["measure_timestamp"].toInt(-1);
          if (timestamp < 0) {
            qWarning() << "invalid timestamp value: "
                       << point["measure_timestamp"].toString();
            return;
          }
          if (!point["measure_value"].isDouble()) {
            qWarning() << "invalid measure_value value: "
                       << point["measure_value"].toString();
            return;
          }
          auto value = point["measure_value"].toDouble(-1001);

          x.append(timestamp);
          y.append(value);
        }
      }
    }

    emit vehiclesHistoryDataReady(x, y);
  });
}

void EgRestDataSource::onAddNewVehicle(QString &vehName, QString &plateNo) {
  QUrl url(m_serverPath + m_addVehiclePath);
  QUrlQuery uq;

  uq.addQueryItem("veh_name", vehName);
  uq.addQueryItem("plate_no", plateNo);
  uq.addQueryItem("user_id", QString::number(1));

  url.setQuery(uq);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr.get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    QVariant status_code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) {
      if (status_code == 200) {
        this->getVehicleList();
      } else {
        qWarning() << "addVehicle request failed with code " << status_code;
      }
    }
  });
}

void EgRestDataSource::assignVehicleToDevice(EgDeviceData *selectedDevice,
                                             EgVehicleData *selectedVehicle) {
  QString devId = selectedDevice->id;
  QString vehId = "";
  if (selectedVehicle) {
    vehId = QString::number(selectedVehicle->id);
  }

  QUrl url(m_serverPath + m_assignDeviceToVehiclesSubpath);
  QUrlQuery uq;

  uq.addQueryItem("device_id", devId);
  uq.addQueryItem("vehicle_id", vehId);
  uq.addQueryItem("user_id", QString::number(1));

  url.setQuery(uq);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr.get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    QVariant status_code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) {
      if (status_code == 200) {
        this->getDevicesList();
      } else {
        qWarning() << "assignVehicleToDevice request failed with code "
                   << status_code;
      }
    }
  });
}

void EgRestDataSource::setServerState(bool newServerState) {
  if (m_serverState == newServerState)
    return;
  m_serverState = newServerState;
  emit serverStateChanged(m_serverState);
}

void EgRestDataSource::serverStateCheck() {
  qDebug() << "REST periodic server check task";

  QUrl url(m_serverPath + m_getUptimeSubpath);
  QNetworkRequest request(url);
  request.setTransferTimeout(2000);
  QNetworkReply *reply = m_netAccMgr.get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    QVariant status_code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) {
      if (status_code == 200) {
        bool ok;
        int uptime = reply->readAll().toInt(&ok);
        if (ok) {
          this->m_uptime = uptime;
          this->m_serverStateTimeoutTimer->start(m_serverStateTimeout);
          setServerState(true);
        } else {
          qWarning() << "Unable to parse getTimestamp REST payload: "
                     << reply->readAll();
        }
      } else {
        qWarning() << "assignVehicleToDevice request failed with code "
                   << status_code;
      }
    }
  });
}

void EgRestDataSource::serverTimeout() {
  qWarning() << "MQTT Server timeout";
  setServerState(false);
}
void EgRestDataSource::onVehicleListRequestReady(QByteArray &replyData) {
  qDebug() << "Received vehicles data: " << replyData;

  auto jsonDoc = QJsonDocument::fromJson(replyData);
  if (!jsonDoc.isArray()) {
    qWarning() << "Vehicle list is not array. Received data: " << replyData;
    return;
  }

  auto vehArray = jsonDoc.array();
  EgVehicleListData vehicleList;
  foreach (auto jsonVeh, vehArray) {
    auto vehData = new EgVehicleData();

    vehData->id = jsonVeh["vehicle_id"].toInt(-1);
    vehData->name = jsonVeh["vehicle_name"].toString("Unknown");
    vehData->plateNo = jsonVeh["vehicle_plate"].toString("Unknown");

    qDebug() << "Parsed vehicle data: " << vehData->id << vehData->name
             << vehData->plateNo;

    vehicleList.vehicles.append(vehData);
  }

  emit vehicleListReady(vehicleList);
}

void EgRestDataSource::onDevicesListRequestReady(QByteArray &replyData) {
  qDebug() << "Received devices data: " << replyData;

  auto jsonDoc = QJsonDocument::fromJson(replyData);
  if (!jsonDoc.isArray()) {
    qWarning() << "Devices list is not array. Received data: " << replyData;
    return;
  }

  auto devArray = jsonDoc.array();
  EgDevicesListData devicesList;
  foreach (auto jsonDev, devArray) {
    auto devData = new EgDeviceData();

    devData->id = jsonDev["device_id"].toString("Unknown");
    devData->type = jsonDev["device_type"].toString("Unknown");
    devData->vehicle_id = jsonDev["vehicle_id"].toInt(-1);
    devData->user_id = jsonDev["user_id"].toInt(-1);

    qDebug() << "Parsed device data: " << devData->id << " " << devData->type
             << " " << devData->vehicle_id << " " << devData->user_id;

    devicesList.devices.append(devData);
  }

  emit deviceListReady(devicesList);
}
