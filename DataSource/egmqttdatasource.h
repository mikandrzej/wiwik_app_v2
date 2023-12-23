#ifndef EGMQTTDATASOURCE_H
#define EGMQTTDATASOURCE_H

#include "EgVehicleData.h"
#include <QObject>

class QMqttClient;
class QTimer;

class EgMqttDataSource : public QObject
{
    Q_OBJECT
  public:
    explicit EgMqttDataSource(QObject* parent = nullptr);

  private slots:
    void serverTimeout();
    void onMqttConnected();
    void onMqttDisconnected();
    void onMqttVehiclesTopicMessageReceived(const QByteArray& msg, const QStringList& topic);
    void onMqttServerMessageReceived(const QByteArray& msg, const QStringList& topic);
    void onMqttDeviceMessageReceived(const QByteArray& msg, const QStringList& topic);
  signals:
    void sensorDataReceived(EgVehicleSensorData& sensorData);
    void serverStateChanged(bool state);

  private:
    QString m_mqttServer = "iot.2canit.pl";
    int m_mqttServerPort = 1883;
    int m_uptime;
    int m_serverTimeout = 30000;
    int m_serverConnRetryInterval = 5000;
    bool m_serverState = false;
    QTimer* m_serverTimeoutTimer = nullptr;
    QMqttClient* m_mqttClient;
    void parseServiceServerMesssage(QString& parameter, const QByteArray& message);
    void setServerState(bool newServerState);
    void parseVehicleMessage(int vehicle_id, const QStringList& topicLevels, const QByteArray& message);
};

#endif   // EGMQTTDATASOURCE_H
