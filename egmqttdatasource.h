#ifndef EGMQTTDATASOURCE_H
#define EGMQTTDATASOURCE_H

#include "EgVehicleData.h"
#include <QMqttClient>
#include <QObject>
#include <QTimer>

class EgMqttDataSource : public QMqttClient
{
    Q_OBJECT
  public:
    explicit EgMqttDataSource(QObject* parent = nullptr);

  private slots:
    void onClentStateChanged(QMqttClient::ClientState state);
    void onVehiclesMessageReceived(QMqttMessage msg);

    void onServerMessageReceived(QMqttMessage msg);
    void serverTimeout();
  signals:
    void sensorDataReceived(EgSensorData& sensorData);
    void serverStateChanged(bool state);

  private:
    int m_uptime;
    int m_serverTimeout = 30000;
    int m_serverConnRetryInterval = 5000;
    bool m_serverState = false;
    QTimer* m_serverTimeoutTimer = nullptr;
    void parseVehicleMessage(int vehicle_id, QStringList& topicLevels, const QByteArray& message);
    void parseServiceServerMesssage(QString& parameter, const QByteArray& message);
    void setServerState(bool newServerState);
};

#endif   // EGMQTTDATASOURCE_H
