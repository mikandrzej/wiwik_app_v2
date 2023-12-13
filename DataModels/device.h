#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class QNetworkAccessManager;
class Vehicle;
class Sensor;

class Device : public QObject {
  Q_OBJECT
public:
  explicit Device(int id, QString &serialNo, QString &name, int vehicleId,
                  QString &type, QObject *parent = nullptr);
  explicit Device(const Device &device, QObject *parent = nullptr);

  int id() const;

  QString serialNo() const;

  QString name() const;
  void setName(const QString &newName);

  int vehicleId() const;
  void setVehicleId(int newVehicleId);

  QString type() const;

  void commitChanges();

  bool changesPending() const;
  void setChangesPending(bool newChangesPending);

  void addSensor(Sensor *sensor);

  void removeSensor(Sensor *sensor);

  Vehicle *vehicle() const;
  void setVehicle(Vehicle *newVehicle);

  signals:
  void nameChanged();

  void vehicleIdChanged();

  void changesPendingChanged();

  void vehicleChanged();

  private:
  int m_id = -1;
  QString m_serialNo = "";
  QString m_name = "";

  int m_vehicleId = -1;
  QString m_type = "";

  bool m_changesPending = false;

  QList<Sensor *> m_sensors;

  Vehicle *m_vehicle;

  QNetworkAccessManager *m_netAccMgr = nullptr;

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(
      int vehicleId READ vehicleId WRITE setVehicleId NOTIFY vehicleIdChanged)
  Q_PROPERTY(bool changesPending READ changesPending WRITE setChangesPending
                 NOTIFY changesPendingChanged)
  Q_PROPERTY(Vehicle *vehicle READ vehicle WRITE setVehicle NOTIFY vehicleChanged FINAL)
};

#endif // DEVICE_H
