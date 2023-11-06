#ifndef ONEVEHICLEBATTERYMODEL_H
#define ONEVEHICLEBATTERYMODEL_H

#include <QMap>
#include <QObject>

class OneVehicleBatteryModel : public QObject {
  Q_OBJECT
public:
  explicit OneVehicleBatteryModel(QObject *parent = nullptr);

  double lastValue() const;

public slots:
  void insert(int timestamp, double value);
signals:

  void lastValueChanged();
  void dataChanged();

private:
  struct BatteryData {
    int timestamp;
    double value;
  };
  QMap<int, BatteryData *> m_data;
  Q_PROPERTY(double lastValue READ lastValue NOTIFY lastValueChanged)
};

#endif // ONEVEHICLEBATTERYMODEL_H
