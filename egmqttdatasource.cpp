#include "egmqttdatasource.h"
#include "EgVehicleData.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

EgMqttDataSource::EgMqttDataSource(QObject *parent) : QMqttClient{parent} {

  setHostname("iot.2canit.pl");
  setPort(1883);
  connectToHost();

  m_serverTimeoutTimer = new QTimer(this);
  m_serverTimeoutTimer->setSingleShot(true);
  connect(m_serverTimeoutTimer, &QTimer::timeout, this,
          &EgMqttDataSource::serverTimeout);

  connect(this, &QMqttClient::stateChanged, this,
          &EgMqttDataSource::onClentStateChanged);
}

void EgMqttDataSource::onClentStateChanged(QMqttClient::ClientState state) {

  if (Connected == state) {
    setServerState(true);
    qDebug() << "Connected";

    auto subsVeh = subscribe(QMqttTopicFilter("vehicles/#"));
    connect(subsVeh, &QMqttSubscription::messageReceived, this,
            &EgMqttDataSource::onVehiclesMessageReceived);

    auto subsServer = subscribe(QMqttTopicFilter("server/#"));
    connect(subsServer, &QMqttSubscription::messageReceived, this,
            &EgMqttDataSource::onServerMessageReceived);
  } else if (Disconnected == state) {
    setServerState(false);
    m_serverTimeoutTimer->stop();
    QTimer::singleShot(m_serverConnRetryInterval, this, [this]() {
      qDebug() << "Reconnecting to MQTT server";
      connectToHost();
    });
  }
}

void EgMqttDataSource::onVehiclesMessageReceived(QMqttMessage msg) {
  qDebug() << "Received MQTT message. Topic: " + msg.topic().name() + " msg " +
                  msg.payload();

  QStringList topicLevels = msg.topic().name().split('/');

  if (topicLevels[0] != "vehicles") {
    qWarning() << "Invalid vehicle mqtt message topic level[0]. Received: "
               << msg.topic().name();
    return;
  }

  bool ok;

  int vehicleId = topicLevels[1].toInt(&ok);
  if (!ok) {
    qWarning() << "Unable to parse vehicleId: " << topicLevels[1];
    return;
  }

  this->parseVehicleMessage(vehicleId, topicLevels, msg.payload());
}

void EgMqttDataSource::onServerMessageReceived(QMqttMessage msg) {

  qDebug() << "Received MQTT message. Topic: " + msg.topic().name() + " msg " +
                  msg.payload();

  QStringList topicLevels = msg.topic().name().split('/');

  if (topicLevels[0] != "server") {
    qWarning() << "Invalid server mqtt message topic level[0]. Received: "
               << msg.topic().name();
    return;
  }

  if (topicLevels.count() != 2) {
    qWarning() << "server topic not supported: " << topicLevels;
    return;
  }
  parseServiceServerMesssage(topicLevels[1], msg.payload());
}

void EgMqttDataSource::parseServiceServerMesssage(QString &parameter,
                                                  const QByteArray &message) {
  if (parameter == "uptime") {
    bool ok;
    int uptime = message.toInt(&ok);
    if (ok) {
      m_uptime = uptime;
      setServerState(true);

      m_serverTimeoutTimer->start(m_serverTimeout);

    } else {
      qWarning() << "Uptime message is not a number" << message;
    }
  } else {
    qWarning() << "Unsupported service server parameter: " << parameter;
  }
}

void EgMqttDataSource::serverTimeout() {
  qWarning() << "MQTT Server timeout";
  setServerState(false);
}

void EgMqttDataSource::setServerState(bool newServerState) {
  if (m_serverState == newServerState)
    return;
  m_serverState = newServerState;
  emit serverStateChanged(m_serverState);
}

void EgMqttDataSource::parseVehicleMessage(int vehicle_id,
                                           QStringList &topicLevels,
                                           const QByteArray &message) {
  QJsonParseError parseErr;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(message, &parseErr);
  if (QJsonParseError::NoError != parseErr.error) {
    qWarning() << "Unable to parse json: " << parseErr.errorString();
    return;
  }

  if (topicLevels.length() != 7)
    return;

  if (topicLevels[2] == "devices") {
    auto deviceAddress = topicLevels[3];
    if (topicLevels[4] == "sensors") {
      auto sensorType = topicLevels[5];
      auto sensorAddress = topicLevels[6];

      auto jsonObj = jsonDoc.object();

      if (!jsonObj.contains("timestamp")) {
        qWarning() << "Json should contain value \"timestamp\"";
        return;
      }
      quint64 timestamp = jsonObj["timestamp"].toInt();

      if (sensorType == "temperature") {
        if (!jsonObj.contains("value")) {
          qWarning() << "Json value for message type:" << sensorType
                     << " should contain key \"value\"";
          return;
        }
        double temperature = jsonObj["value"].toDouble();
        EgSensorData sensorData = {
            .sensorAddress = sensorAddress,
            .vehicleId = vehicle_id,
            .dataType = EgSensorDataType::Temperature,
            .timestamp = QDateTime::fromSecsSinceEpoch(timestamp),
            .temperature = temperature,
        };
        emit sensorDataReceived(sensorData);
      } else if (sensorType == "battery") {
        if (!jsonObj.contains("value")) {
          qWarning() << "Json value for sensor type:" << sensorType
                     << " should contain key \"value\"";
          return;
        }
        double battery = jsonObj["value"].toDouble();
        EgSensorData sensorData = {
            .sensorAddress = sensorAddress,
            .vehicleId = vehicle_id,
            .dataType = EgSensorDataType::BatteryVoltage,
            .timestamp = QDateTime::fromSecsSinceEpoch(timestamp),
            .battery = battery,
        };
        emit sensorDataReceived(sensorData);
      } else if (sensorType == "gps") {
        int fix_mode = jsonObj["fix_mode"].toInt();
        int satellites = jsonObj["satellites"].toInt();
        double latitude = jsonObj["latitude"].toDouble();
        double longitude = jsonObj["longitude"].toDouble();
        double speed = jsonObj["speed"].toDouble();
        double precision = jsonObj["precision"].toDouble();
        int gps_timestamp = jsonObj["gps_timestamp"].toInt();
        QGeoPositionInfo positionInfo;
        positionInfo.setCoordinate(QGeoCoordinate(latitude, longitude));
        positionInfo.setAttribute(QGeoPositionInfo::GroundSpeed, speed);
        positionInfo.setAttribute(QGeoPositionInfo::HorizontalAccuracy,
                                  precision);
        positionInfo.setTimestamp(QDateTime::fromSecsSinceEpoch(gps_timestamp));

        EgSensorData sensorData = {.sensorAddress = sensorAddress,
                                   .deviceAddress = deviceAddress,
                                   .vehicleId = vehicle_id,
                                   .dataType = EgSensorDataType::GpsPosition,
                                   .timestamp =
                                       QDateTime::fromSecsSinceEpoch(timestamp),
                                   .geoPosition = positionInfo};
        emit sensorDataReceived(sensorData);
      }
    }
  }
}
