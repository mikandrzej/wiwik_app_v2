#ifndef ONEVEHICLEMODEL_H
#define ONEVEHICLEMODEL_H

#include "onevehiclebatterymodel.h"
#include "onevehiclepositionmodel.h"
#include "onevehicletemperaturemodel.h"

#include <QObject>

class OneVehicleModel : public QObject {
  Q_OBJECT
public:
  explicit OneVehicleModel(QObject *parent = nullptr);

  QString name() const;
  void setName(const QString &newName);

  QString plateNo() const;
  void setPlateNo(const QString &newPlateNo);

  OneVehiclePositionModel *positionModel() const;

  OneVehicleTemperatureModel *temperatureModel() const;

  OneVehicleBatteryModel *batteryModel() const;

  int id() const;
  void setId(int newId);

signals:

  void nameChanged();

  void plateNoChanged();

  void positionModelChanged();

  void temperatureModelChanged();

  void batteryModelChanged();

  void idChanged();

private:
  int m_id;
  QString m_name;
  QString m_plateNo;
  OneVehicleTemperatureModel *m_temperatureModel =
      new OneVehicleTemperatureModel(this);
  OneVehiclePositionModel *m_position = new OneVehiclePositionModel(this);
  OneVehicleBatteryModel *m_battery = new OneVehicleBatteryModel(this);

  Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(
      QString plateNo READ plateNo WRITE setPlateNo NOTIFY plateNoChanged)
  Q_PROPERTY(OneVehicleTemperatureModel *temperatureModel READ temperatureModel
                 NOTIFY temperatureModelChanged)
  Q_PROPERTY(OneVehiclePositionModel *positionModel READ positionModel NOTIFY
                 positionModelChanged)
  Q_PROPERTY(OneVehicleBatteryModel *batteryModel READ batteryModel NOTIFY
                 batteryModelChanged)
};

#endif // ONEVEHICLEMODEL_H
