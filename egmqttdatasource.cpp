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
    qDebug() << "Connected";

    auto subsVeh = subscribe(QMqttTopicFilter("vehicles/#"));
    connect(subsVeh, &QMqttSubscription::messageReceived, this,
            &EgMqttDataSource::onVehiclesMessageReceived);

    auto subsServer = subscribe(QMqttTopicFilter("server/#"));
    connect(subsServer, &QMqttSubscription::messageReceived, this,
            &EgMqttDataSource::onServerMessageReceived);
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

  auto jsonObj = jsonDoc.object();

  if (!jsonObj.contains("timestamp")) {
    qWarning() << "Json should contain value \"timestamp\"";
    return;
  }
  quint64 timestamp = jsonObj["timestamp"].toInt();
  QString message_type = topicLevels[2];

  if (!jsonObj.contains("value")) {
    qWarning() << "Json value for message type:" << message_type
               << " should contain key \"value\"";
    return;
  }

  if (message_type == "irvine_temperature1") {
    double temperature = jsonObj["value"].toDouble();
    qDebug() << "Received temperature data " << temperature
             << " from vehicle:" << vehicle_id;
    EgSensorData sensorData = {
        .vehicleId = vehicle_id,
        .timestamp = QDateTime::fromSecsSinceEpoch(timestamp),
        .temperature = temperature,
    };
    emit sensorDataReceived(sensorData);
  } else if (message_type == "irvine_battery") {
    double battery = jsonObj["value"].toDouble();
    qDebug() << "Received battery data " << battery
             << " from vehicle:" << vehicle_id;
  } else {
    qWarning() << "Unknown type of vehicle message: " << message_type;
    return;
  }
}
