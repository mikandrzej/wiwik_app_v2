#ifndef ONEVEHICLEPOSITIONMODEL_H
#define ONEVEHICLEPOSITIONMODEL_H

#include <QGeoPositionInfo>
#include <QMap>
#include <QObject>

class OneVehiclePositionModel : public QObject {
  Q_OBJECT
public:
  explicit OneVehiclePositionModel(QObject *parent = nullptr);

  QGeoPositionInfo lastValue() const;

  QVariantList path();

public slots:
  void insert(int timestamp, QGeoPositionInfo &value);

signals:

  void lastValueChanged();
  void dataChanged();

private:
  struct PositionData {
    int timestamp;
    QGeoPositionInfo value;
  };
  QMap<int, PositionData *> m_data;
  Q_PROPERTY(QGeoPositionInfo lastValue READ lastValue NOTIFY lastValueChanged)
};

#endif // ONEVEHICLEPOSITIONMODEL_H
