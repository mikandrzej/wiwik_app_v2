#ifndef EGRESTDATASOURCE_H
#define EGRESTDATASOURCE_H

#include "EgDeviceData.h"
#include "EgVehicleData.h"
#include <QObject>

class QNetworkAccessManager;
class QTimer;
class EgRestDataSource : public QObject {
  Q_OBJECT

public:
  explicit EgRestDataSource(QObject *parent = nullptr);

  void getVehicleList();
  void getDevicesList();
  void getSensorsList();
  void assignVehicleToDevice(EgDeviceData *selectedDevice,
                             EgVehicleData *selectedVehicle);

public slots:
  void requestTemperatureHistoryData(int vehicleId, QDate &date);
  void onAddNewVehicle(QString &vehName, QString &plateNo);
  void onEditVehicle(int id, QString &vehName, QString &plateNo);
signals:
  void vehicleListReady(EgVehiclesData &vehicleListData);
  void deviceListReady(EgDevicesListData &vehicleListData);
  void sensorListReady(EgSensorsListData &sensorListData);
  void serverStateChanged(bool state);
  void vehiclesHistoryDataReady(EgTemperatureListData &);

private slots:
  void serverTimeout();

private:
  QNetworkAccessManager *m_netAccMgr;

  //  QString m_serverPath = "http://iot.2canit.pl:5000";
  QString m_serverPath = "http://10.22.1.57:14999";
  QString m_getVehiclesSubpath = "/api/getVehicles";
  QString m_getDevicesSubpath = "/api/getDevices";
  QString m_getSensorsSubpath = "/api/getSensors";
  QString m_setDeviceDataSubpath = "/api/setDeviceData";
  QString m_getUptimeSubpath = "/api/getUptime";
  QString m_getTemperatureDataSubpath = "/api/getTemperatureData";
  QString m_addVehiclePath = "/api/addVehicle";
  QString m_setVehicleDataSubpath = "/api/setVehicleData";

  int m_uptime;
  int m_serverStatePeriod = 3000;
  int m_serverStateTimeout = 10000;
  bool m_serverState = false;
  QTimer *m_serverStateTimer = nullptr;
  QTimer *m_serverStateTimeoutTimer = nullptr;

  void serverStateCheck();
  void setServerState(bool newServerState);

  void onVehicleListRequestReady(QByteArray &replyData);
  void onDevicesListRequestReady(QByteArray &replyData);
  void onSensorsListRequestReady(QByteArray &replyData);
};

#endif // EGRESTDATASOURCE_H
