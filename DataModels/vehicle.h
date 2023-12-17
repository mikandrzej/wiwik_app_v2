#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>

class QNetworkAccessManager;
class Device;

class Vehicle : public QObject {
  Q_OBJECT
public:
  explicit Vehicle(int id, QString &name, QString &plateNo,
                   QObject *parent = nullptr);
  explicit Vehicle(const Vehicle &vehicle, QObject *parent = nullptr);

  int id() const;

  QString name() const;
  void setName(const QString &newName);

  QString plateNo() const;
  void setPlateNo(const QString &newPlateNo);

  void commitChanges();

  bool changesPending() const;
  void setChangesPending(bool newChangesPending);

  void addDevice(Device *device);
  void removeDevice(Device *device);

  QList<Device *> devices() const;

  signals:
  void nameChanged();

  void plateNoChanged();

  void changesPendingChanged();
  void deviceAdded(Device *device, int index);
  void deviceRemoved(int index);

  private:
  int m_id;
  QString m_name;
  QString m_plateNo;

  bool m_changesPending = false;

  QList<Device *> m_devices = {};

  QNetworkAccessManager *m_netAccMgr = nullptr;

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(
      QString plateNo READ plateNo WRITE setPlateNo NOTIFY plateNoChanged)
  Q_PROPERTY(bool changesPending READ changesPending WRITE setChangesPending
                 NOTIFY changesPendingChanged)
};

#endif // VEHICLE_H
