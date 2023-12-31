#include "egmqttdatasource.h"
#include "../DataModels/gpsdata.h"
#include "../DataModels/measure.h"
#include "../DataSource/datacontainer.h"
#include "EgVehicleData.h"
#include <QDebug>
#include <QGeoLocation>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMqttClient>
#include <QTimer>

EgMqttDataSource::EgMqttDataSource(QObject* /*parent*/)
{
    m_mqttClient = new QMqttClient(this);

    m_mqttClient->setHostname(m_mqttServer);
    m_mqttClient->setPort(m_mqttServerPort);
    m_mqttClient->connectToHost();

    m_serverTimeoutTimer = new QTimer(this);
    m_serverTimeoutTimer->setSingleShot(true);
    connect(m_serverTimeoutTimer, &QTimer::timeout, this, &EgMqttDataSource::serverTimeout);

    connect(m_mqttClient, &QMqttClient::connected, this, &EgMqttDataSource::onMqttConnected);
    connect(m_mqttClient, &QMqttClient::disconnected, this, &EgMqttDataSource::onMqttDisconnected);
}

void EgMqttDataSource::onMqttConnected()
{
    setServerState(true);
    qDebug() << "Connected";

    auto subsVeh = m_mqttClient->subscribe(QMqttTopicFilter("vehicles/#"));
    connect(subsVeh, &QMqttSubscription::messageReceived, this, [this](const QMqttMessage& msg) {
        this->onMqttVehiclesTopicMessageReceived(msg.payload(), msg.topic().levels());
    });

    auto subsServer = m_mqttClient->subscribe(QMqttTopicFilter("server/#"));
    connect(
        subsServer, &QMqttSubscription::messageReceived, this, [this](const QMqttMessage& msg) { this->onMqttServerMessageReceived(msg.payload(), msg.topic().levels()); });

    auto subsIrvine = m_mqttClient->subscribe(QMqttTopicFilter("devices/#"));
    connect(subsIrvine, &QMqttSubscription::messageReceived, this, [this](const QMqttMessage& msg) {
        auto topicLevels = msg.topic().levels();
        topicLevels.removeFirst();
        this->onMqttDeviceMessageReceived(msg.payload(), topicLevels);
    });
}

void EgMqttDataSource::onMqttDisconnected()
{
    setServerState(false);
    m_serverTimeoutTimer->stop();
    QTimer::singleShot(m_serverConnRetryInterval, this, [this]() {
        qDebug() << "Reconnecting to MQTT server";
        m_mqttClient->connectToHost();
    });
}

void EgMqttDataSource::onMqttVehiclesTopicMessageReceived(const QByteArray& msg, const QStringList& topic)
{
    if (topic[0] != "vehicles")
    {
        qWarning() << "Invalid vehicle mqtt message topic level[0]. Received: " << topic[0];
        return;
    }

    bool ok;

    int vehicleId = topic[1].toInt(&ok);
    if (!ok)
    {
        qWarning() << "Unable to parse vehicleId: " << topic[1];
        return;
    }

    this->parseVehicleMessage(vehicleId, topic, msg);
}

void EgMqttDataSource::parseVehicleMessage(int vehicle_id, const QStringList& topicLevels, const QByteArray& message)
{
    QJsonParseError parseErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message, &parseErr);
    if (QJsonParseError::NoError != parseErr.error)
    {
        qWarning() << "Unable to parse json: " << parseErr.errorString();
        return;
    }

    if (topicLevels.length() != 7)
        return;

    if (topicLevels[2] == "devices")
    {
        const auto& deviceAddress = topicLevels[3];
        if (topicLevels[4] == "sensors")
        {
            const auto& sensorType = topicLevels[5];
            const auto& sensorAddress = topicLevels[6];

            auto jsonObj = jsonDoc.object();

            if (!jsonObj.contains("timestamp"))
            {
                qWarning() << "Json should contain value \"timestamp\"";
                return;
            }
            quint64 timestamp = jsonObj["timestamp"].toInt();

            if (sensorType == "temperature")
            {
                if (!jsonObj.contains("value"))
                {
                    qWarning() << "Json value for message type:" << sensorType << " should contain key \"value\"";
                    return;
                }
                double temperature = jsonObj["value"].toDouble();
                EgVehicleSensorData sensorData = {
                    .sensorAddress = sensorAddress,
                    .vehicleId = vehicle_id,
                    .dataType = EgSensorDataType::Temperature,
                    .timestamp = QDateTime::fromSecsSinceEpoch(timestamp),
                    .temperature = temperature,
                };
                emit sensorDataReceived(sensorData);
                /////////

                // Measure measure(sensorData.timestamp, temperature);
                // DataContainer::instance()->insertNewMeasure(sensorData.sensorAddress, measure);

                ///////
            }
            else if (sensorType == "battery")
            {
                if (!jsonObj.contains("value"))
                {
                    qWarning() << "Json value for sensor type:" << sensorType << " should contain key \"value\"";
                    return;
                }
                double battery = jsonObj["value"].toDouble();
                EgVehicleSensorData sensorData = {
                    .sensorAddress = sensorAddress,
                    .vehicleId = vehicle_id,
                    .dataType = EgSensorDataType::BatteryVoltage,
                    .timestamp = QDateTime::fromSecsSinceEpoch(timestamp),
                    .battery = battery,
                };
                emit sensorDataReceived(sensorData);

                /////////

                // Measure measure(sensorData.timestamp, battery);
                // DataContainer::instance()->insertNewMeasure(sensorData.sensorAddress, measure);

                ///////
            }
            else if (sensorType == "gps")
            {
                int satellites = jsonObj["satellites"].toInt();
                double latitude = jsonObj["latitude"].toDouble();
                double longitude = jsonObj["longitude"].toDouble();
                double speed = jsonObj["speed"].toDouble();
                double precision = jsonObj["precision"].toDouble();
                int gps_timestamp = jsonObj["gps_timestamp"].toInt();
                QString address = jsonObj["address"].toString();
                QGeoPositionInfo positionInfo;
                positionInfo.setCoordinate(QGeoCoordinate(latitude, longitude));
                positionInfo.setAttribute(QGeoPositionInfo::GroundSpeed, speed);
                positionInfo.setAttribute(QGeoPositionInfo::HorizontalAccuracy, precision);
                positionInfo.setTimestamp(QDateTime::fromSecsSinceEpoch(gps_timestamp));

                EgVehicleSensorData sensorData = {.sensorAddress = sensorAddress,
                                                  .deviceAddress = deviceAddress,
                                                  .vehicleId = vehicle_id,
                                                  .dataType = EgSensorDataType::GpsPosition,
                                                  .timestamp = QDateTime::fromSecsSinceEpoch(timestamp),
                                                  .satellites = satellites,
                                                  .geoPosition = positionInfo,
                                                  .address = address};
                emit sensorDataReceived(sensorData);

                /////////

                // Measure measure(sensorData.timestamp, QVariant::fromValue(positionInfo));
                // DataContainer::instance()->insertNewMeasure(sensorData.sensorAddress, measure);

                ///////
            }
        }
    }
}

void EgMqttDataSource::onMqttServerMessageReceived(const QByteArray& msg, const QStringList& topic)
{
    if (topic[0] != "server")
    {
        qWarning() << "Invalid server mqtt message topic level[0]. Received: " << topic[0];
        return;
    }

    if (topic.count() != 2)
    {
        qWarning() << "server topic not supported: " << topic;
        return;
    }

    if (topic[1] == "uptime")
    {
        bool ok;
        int uptime = msg.toInt(&ok);
        if (ok)
        {
            m_uptime = uptime;
            setServerState(true);

            m_serverTimeoutTimer->start(m_serverTimeout);
        }
        else
        {
            qWarning() << "Uptime message is not a number" << msg;
        }
    }
    else
    {
        qWarning() << "Unsupported service server parameter: " << topic[1];
    }
}

void EgMqttDataSource::onMqttDeviceMessageReceived(const QByteArray& msg, const QStringList& topic)
{
    if (topic.length() < 3)
        return;
    // const auto& deviceAddress = topic[0];
    const auto& sensorType = topic[1];
    auto sensorAddress = topic[2];

    QJsonParseError parseErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(msg, &parseErr);
    if (QJsonParseError::NoError != parseErr.error)
    {
        qWarning() << "Unable to parse json: " << parseErr.errorString();
        return;
    }
    auto jsonObj = jsonDoc.object();
    if (!jsonObj.contains("timestamp"))
    {
        qWarning() << "Json value for message type:" << sensorType << " should contain key \"timestamp\"";
        return;
    }
    // auto timestamp = QDateTime::fromSecsSinceEpoch(jsonObj["timestamp"].toInt());
    auto timestampStr = jsonObj["timestamp"].toString();
    auto timestamp = dateTimeFromString(timestampStr);

    Measure* measure = nullptr;

    if (sensorType == "temperature")
    {
        if (!jsonObj.contains("value"))
        {
            qWarning() << "Json value for message type:" << sensorType << " should contain key \"value\"";
            return;
        }
        double temperature = jsonObj["value"].toDouble();
        measure = new Measure(timestamp, temperature);
    }
    else if (sensorType == "battery")
    {
        if (!jsonObj.contains("value"))
        {
            qWarning() << "Json value for message type:" << sensorType << " should contain key \"value\"";
            return;
        }
        double battery = jsonObj["value"].toDouble();
        measure = new Measure(timestamp, battery);
    }
    else if (sensorType == "gps")
    {
        double latitude = jsonObj["latitude"].toDouble();
        double longitude = jsonObj["longitude"].toDouble();
        double altitude = jsonObj["altitude"].toDouble();
        double precision = jsonObj["precision"].toDouble();
        double speed = jsonObj["speed"].toDouble();

        QString country = jsonObj["country"].toString();
        QString postcode = jsonObj["postcode"].toString();
        QString village = jsonObj["village"].toString();
        QString road = jsonObj["road"].toString();

        auto gpsTimestampStr = jsonObj["gps_timestamp"].toString();
        auto gpsTimestamp = dateTimeFromString(gpsTimestampStr);

        GpsData gpsData;

        gpsData.setCoordinate(QGeoCoordinate(latitude, longitude, altitude));

        if (country.length() > 0)
        {
            QGeoAddress geoAddress;
            geoAddress.setStreet(road);
            geoAddress.setCity(village);
            geoAddress.setPostalCode(postcode);
            geoAddress.setCountry(country);
            gpsData.setAddress(geoAddress);
        }
        else
        {
            qDebug() << "Invalid address data: " << country << " " << postcode << " " << village << " " << road;
        }

        gpsData.setAccuracy(precision);
        gpsData.setSpeed(speed);
        gpsData.setGpsTimestamp(gpsTimestamp);

        measure = new Measure(timestamp, QVariant::fromValue(gpsData));
    }
    else
    {
        qWarning() << "Unknown sensor type: " << sensorType;
        return;
    }

    if (measure != nullptr)
    {
        DataContainer::instance()->insertNewMeasure(sensorAddress, *measure);
        delete measure;
    }
}

void EgMqttDataSource::serverTimeout()
{
    qWarning() << "MQTT Server timeout";
    setServerState(false);
}

void EgMqttDataSource::setServerState(bool newServerState)
{
    if (m_serverState == newServerState)
        return;
    m_serverState = newServerState;
    emit serverStateChanged(m_serverState);
}

QDateTime EgMqttDataSource::dateTimeFromString(QString& text)
{
    auto splTimestampStr = text.split(" GMT");
    if (splTimestampStr.length() != 2)
    {
        qWarning() << "Invalid timestamp str: \"" << text;
        return QDateTime();
    }
    auto dateTimeStr = splTimestampStr[0];
    auto hourOffset = splTimestampStr[1].toInt();
    auto timestamp = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm:ss");
    timestamp.setOffsetFromUtc(hourOffset * 60 * 60);

    return timestamp;
}
