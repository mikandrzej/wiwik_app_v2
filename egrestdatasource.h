#ifndef EGRESTDATASOURCE_H
#define EGRESTDATASOURCE_H

#include "EgDeviceData.h"
#include "EgVehicleData.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class EgRestDataSource : public QObject {
  Q_OBJECT

public:
  explicit EgRestDataSource(QObject *parent = nullptr);

  void getVehicleList();
  void getDevicesList();
  void assignVehicleToDevice(EgDeviceData *selectedDevice,
                             EgVehicleData *selectedVehicle);

public slots:
  void requestVehicleHistoryData(int vehicleId, QDate &date);
  void onAddNewVehicle(QString &vehName, QString &plateNo);
signals:
  void vehicleListReady(EgVehiclesData &vehicleListData);
  void deviceListReady(EgDevicesListData &vehicleListData);
  void serverStateChanged(bool state);
  void vehiclesHistoryDataReady(QVector<int> &, QVector<double> &);

private slots:
  void serverTimeout();

private:
  QNetworkAccessManager m_netAccMgr;

  QString m_serverPath = "http://iot.2canit.pl:5000";
  QString m_getVehiclesSubpath = "/api/getVehicles";
  QString m_getDevicesSubpath = "/api/getDevices";
  QString m_assignDeviceToVehiclesSubpath = "/api/assignDeviceToVehicle";
  QString m_getUptimeSubpath = "/api/getUptime";
  QString m_getVehicleHistoryDataPath = "/api/getVehicleTempData";
  QString m_addVehiclePath = "/api/addVehicle";

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
};

#endif // EGRESTDATASOURCE_H
