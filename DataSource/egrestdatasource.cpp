#include "egrestdatasource.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>

#include <QDebug>
#include <QNetworkAccessManager>
#include <QTimer>

#include <QTimeZone>

#include "../DataModels/device.h"
#include "../DataModels/sensor.h"
#include "../DataModels/vehicle.h"
#include "datacontainer.h"
#include "qgeoaddress.h"

EgRestDataSource::EgRestDataSource(QObject *parent) : QObject{parent} {
  m_netAccMgr = new QNetworkAccessManager(this);
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
  QNetworkReply *reply = m_netAccMgr->get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto data = reply->readAll();
    this->onVehicleListRequestReady(data);
  });
}

void EgRestDataSource::getDevicesList() {
  QUrl url(m_serverPath + m_getDevicesSubpath);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr->get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto data = reply->readAll();
    this->onDevicesListRequestReady(data);
  });
}

void EgRestDataSource::getSensorsList() {
  QUrl url(m_serverPath + m_getSensorsSubpath);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr->get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto data = reply->readAll();
    this->onSensorsListRequestReady(data);
  });
}

void EgRestDataSource::requestTemperatureHistoryData(int vehicleId,
                                                     QDate &date) {

  QUrl url(m_serverPath + m_getTemperatureDataSubpath);
  QUrlQuery query;
  auto now = QDateTime::currentDateTime();
  auto offset = now.timeZone().offsetFromUtc(now);
  auto dateTimeFrom = date.startOfDay().addSecs(-offset);
  auto dateTimeTo = dateTimeFrom.addDays(1);
  query.addQueryItem("time_from", dateTimeFrom.toString("yyyy-MM-dd hh:mm:ss"));
  query.addQueryItem("time_to", dateTimeTo.toString("yyyy-MM-dd hh:mm:ss"));
  query.addQueryItem("vehicle_id", QString::number(vehicleId));
  url.setQuery(query);

  QNetworkRequest request(url);

  QNetworkReply *reply = m_netAccMgr->get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    auto replyData = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData);

    EgTemperatureListData tempList;

    if (!jsonDoc.isNull()) {
      if (jsonDoc.isArray()) {
        auto pointArray = jsonDoc.array();

        foreach (auto point, pointArray) {
          if (!point["timestamp"].isDouble()) {
            qWarning() << "Not found timestamp key";
            continue;
          }
          auto timestamp = point["timestamp"].toInt(-1);
          if (timestamp < 0) {
            qWarning() << "invalid timestamp value: "
                       << point["timestamp"].toString();
            continue;
          }

          if (!point["value"].isDouble()) {
            qWarning() << "Not found value key";
            continue;
          }
          auto value = point["value"].toDouble(-10010.);
          if (value < -1000.0) {
            qWarning() << "invalid value value: " << point["value"].toString();
            continue;
          }

          QString sensor_name = {""};
          if (point["sensor_name"].isString()) {
            sensor_name = point["sensor_name"].toString();
          }

          if (!point["sensor_addr"].isString()) {
            qWarning() << "Not found sensor_addr key";
            continue;
          }
          auto sensor_addr = point["sensor_addr"].toString();

          auto pack = tempList.sensors[sensor_addr];
          if (!pack) {
            pack = new EgTemperatureData();
            pack->sensor_name = sensor_name;
            pack->sensor_address = sensor_addr;
            tempList.sensors[sensor_addr] = pack;
          }
          pack->timestamps.append(timestamp);
          pack->values.append(value);
        }
      }
    }

    emit vehiclesTemperatureHistoryDataReady(tempList);
  });
}

void EgRestDataSource::requestGpsHistoryData(int vehicleId, QDate &date)
{
    QUrl url(m_serverPath + m_getGpsDataSubpath);
    QUrlQuery query;
    //calculate utc offset
    auto now = QDateTime::currentDateTime();
    auto offset = now.timeZone().offsetFromUtc(now);
    //calculate time boundaries
    auto dateTimeFrom = date.startOfDay().addSecs(-offset);
    auto dateTimeTo = dateTimeFrom.addDays(1);
    // prepare query
    query.addQueryItem("time_from", dateTimeFrom.toString("yyyy-MM-dd hh:mm:ss"));
    query.addQueryItem("time_to", dateTimeTo.toString("yyyy-MM-dd hh:mm:ss"));
    query.addQueryItem("vehicle_id", QString::number(vehicleId));
    url.setQuery(query);

    QNetworkRequest request(url);

    QNetworkReply *reply = m_netAccMgr->get(request);

    connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
        auto replyData = reply->readAll();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData);

        EgGpsListData tempList;

        if (!jsonDoc.isNull()) {
            if (jsonDoc.isArray()) {
                auto dbEntries = jsonDoc.array();

                foreach (auto dbEntry, dbEntries) {
                    auto deviceName = dbEntry["device_name"].toString();
                    auto address = dbEntry["address"].toString();
                    auto timestampRaw = dbEntry["timestamp"].toInt();
                    auto timestamp = QDateTime::fromSecsSinceEpoch(timestampRaw);
                    auto latitude = dbEntry["latitude"].toDouble();
                    auto longitude = dbEntry["longitude"].toDouble();
                    auto sensorAddress = dbEntry["sensor_addr"].toString();
                    auto sensorName = dbEntry["sensor_name"].toString();
                    auto speed = dbEntry["speed"];

                    auto pack = tempList.sensors[sensorAddress];
                    if (!pack) {
                        pack = new EgGpsData();
                        pack->sensor_name = sensorName;
                        pack->sensor_address = sensorAddress;
                        tempList.sensors[sensorAddress] = pack;
                    }
                    QGeoLocation geoLocation;
                    geoLocation.setCoordinate(QGeoCoordinate(latitude, longitude));
                    if (!address.isEmpty()) {
                        auto addressComponents = address.split(", ");
                        if (addressComponents.length() == 8) {
                            QGeoAddress geoAddress;
                            geoAddress.setStreet(addressComponents[1]);
                            geoAddress.setCity(addressComponents[2]);
                            geoAddress.setState(addressComponents[5]);
                            geoAddress.setPostalCode(addressComponents[6]);
                            geoAddress.setCountry(addressComponents[7]);

                            geoLocation.setAddress(geoAddress);
                        }
                    }

                    pack->timestamps.push_back(timestamp);
                    pack->geolocations.push_back(geoLocation);
                }
            }
        }

        emit vehiclesGpsHistoryDataReady(tempList);
    });
}

void EgRestDataSource::onAddNewVehicle(QString &vehName, QString &plateNo) {
  QUrl url(m_serverPath + m_addVehiclePath);
  QUrlQuery uq;

  uq.addQueryItem("name", vehName);
  uq.addQueryItem("plate", plateNo);

  url.setQuery(uq);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr->get(request);

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

void EgRestDataSource::onEditVehicle(int id, QString &vehName,
                                     QString &plateNo) {
  QUrl url(m_serverPath + m_setVehicleDataSubpath);
  QUrlQuery uq;

  uq.addQueryItem("vehicle_id", QString::number(id));
  uq.addQueryItem("name", vehName);
  uq.addQueryItem("plate", plateNo);

  url.setQuery(uq);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr->get(request);

  connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
    QVariant status_code =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) {
      if (status_code == 200) {
        this->getVehicleList();
      } else {
        qWarning() << "editVehicle request failed with code " << status_code;
      }
    }
  });
}

void EgRestDataSource::assignVehicleToDevice(EgDeviceData *selectedDevice,
                                             EgVehicleData *selectedVehicle) {
  QString devId = QString::number(selectedDevice->id);
  QString vehId = "";
  if (selectedVehicle) {
    vehId = QString::number(selectedVehicle->id);
  }

  QUrl url(m_serverPath + m_setDeviceDataSubpath);
  QUrlQuery uq;

  uq.addQueryItem("device_id", devId);
  uq.addQueryItem("vehicle_id", vehId);

  url.setQuery(uq);
  QNetworkRequest request(url);

  // send url request
  QNetworkReply *reply = m_netAccMgr->get(request);

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
  QNetworkReply *reply = m_netAccMgr->get(request);

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
  EgVehiclesData vehicleList;
  foreach (auto jsonVeh, vehArray) {
    auto vehData = new EgVehicleData();

    vehData->id = jsonVeh["id"].toInt(-1);
    vehData->name = jsonVeh["name"].toString("Unknown");
    vehData->plateNo = jsonVeh["plate"].toString("Unknown");
    vehData->color = jsonVeh["color"].toDouble();

    qDebug() << "Parsed vehicle data: " << vehData->id << vehData->name << vehData->plateNo
             << vehData->color;

    vehicleList.vehicles.append(vehData);
  }

  emit vehicleListReady(vehicleList);

  QList<Vehicle *> newVehList;
  for (auto &veh : vehicleList.vehicles) {
      newVehList.append(new Vehicle(veh->id, veh->name, veh->plateNo, veh->color));
  }
  DataContainer::instance()->overwriteVehicles(newVehList);
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

    devData->id = jsonDev["id"].toInt(-1);
    devData->serial_no = jsonDev["serial_no"].toString("");
    devData->name = jsonDev["name"].toString("");
    devData->type = jsonDev["type"].toString("");
    devData->vehicle_id = jsonDev["vehicle_id"].toInt(-1);
    devData->user_id = 1;

    qDebug() << "Parsed device data: " << devData->id << " " << devData->name
             << " " << devData->type << " " << devData->vehicle_id << " "
             << devData->user_id;

    devicesList.devices.append(devData);
  }

  emit deviceListReady(devicesList);

  QList<Device *> newDevList;
  for (auto &dev : devicesList.devices) {
    newDevList.append(new Device(dev->id, dev->serial_no, dev->name,
                                 dev->vehicle_id, dev->type));
  }
  DataContainer::instance()->overwriteDevices(newDevList);
}

void EgRestDataSource::onSensorsListRequestReady(QByteArray &replyData) {
  qDebug() << "Received sensors data: " << replyData;

  auto jsonDoc = QJsonDocument::fromJson(replyData);
  if (!jsonDoc.isArray()) {
    qWarning() << "Sensors list is not array. Received data: " << replyData;
    return;
  }

  auto sensArray = jsonDoc.array();
  EgSensorsListData sensorsList;
  foreach (auto jsonSens, sensArray) {
    auto sensData = new EgSensorData();

    sensData->id = jsonSens["id"].toInt(-1);
    sensData->address = jsonSens["address"].toString("");
    sensData->name = jsonSens["name"].toString("");
    sensData->type = jsonSens["type"].toString("");
    sensData->device_id = jsonSens["device_id"].toInt(-1);
    sensData->device_name = jsonSens["device_name"].toString("");

    qDebug() << "Parsed sensor data: " << sensData->id << " " << sensData->name
             << " " << sensData->type << " " << sensData->address << " "
             << sensData->device_id << sensData->device_name;

    sensorsList.sensors.append(sensData);
  }

  emit sensorListReady(sensorsList);

  QList<Sensor *> newSensorList;
  for (auto &sensor : sensorsList.sensors) {
    newSensorList.append(new Sensor(sensor->id, sensor->name, sensor->address,
                                    sensor->type, sensor->device_id));
  }
  DataContainer::instance()->overwriteSensors(newSensorList);
}
