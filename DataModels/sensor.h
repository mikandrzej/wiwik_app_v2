#ifndef SENSOR_H
#define SENSOR_H

#include "measure.h"
#include <QDateTime>
#include <QObject>

class Device;
class QNetworkAccessManager;

class Sensor : public QObject {
  Q_OBJECT
public:
  explicit Sensor(int id, const QString &name, const QString &address,
                  const QString &type, int deviceId, QObject *parent = nullptr);
  explicit Sensor(const Sensor &sensor, QObject *parent = nullptr);
  explicit Sensor(const QString &address, QObject *parent = nullptr);


  QString name() const;
  void setName(const QString &newName);

  bool changesPending() const;
  void setChangesPending(bool newChangesPending);

  void commitChanges();
  Measure *lastMeasure() const;

  QMap<QDateTime, Measure *> measures() const;
  void setMeasures(const QMap<QDateTime, Measure *> &newMeasures);

  int deviceId() const;
  void setDeviceId(int newDeviceId);

  Device *device() const;
  void setDevice(Device *newDevice);

  int id() const;
  void setId(int newId);

  QString address() const;
  void setAddress(const QString &newAddress);

  QString type() const;
  void setType(const QString &newType);

  public slots:
  void addMeasure(Measure &newLastMeasure);
signals:

  void nameChanged();

  void changesPendingChanged();

  void lastMeasureChanged();

  void measuresChanged();

  void deviceIdChanged();

  void deviceChanged();

  void idChanged();

  void addressChanged();

  void typeChanged();

  private:
  int m_id = -1;
  QString m_name = "";
  QString m_address = "";
  QString m_type = "";
  int m_deviceId = -1;

  Device *m_device = nullptr;

  bool m_changesPending = false;

  QMap<QDateTime, Measure *> m_measures;
  Measure *m_lastMeasure = nullptr;

  QNetworkAccessManager *m_netAccMgr = nullptr;

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(bool changesPending READ changesPending WRITE setChangesPending
                 NOTIFY changesPendingChanged)
  Q_PROPERTY(Measure *lastMeasure READ lastMeasure NOTIFY lastMeasureChanged)
  Q_PROPERTY(QMap<QDateTime, Measure *> measures READ measures WRITE setMeasures
                 NOTIFY measuresChanged)
  Q_PROPERTY(int deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged FINAL)
  Q_PROPERTY(Device *device READ device WRITE setDevice NOTIFY deviceChanged FINAL)
  Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
  Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
  Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged FINAL)
};

#endif // SENSOR_H
