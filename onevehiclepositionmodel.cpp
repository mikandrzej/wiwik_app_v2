#include "onevehiclepositionmodel.h"

OneVehiclePositionModel::OneVehiclePositionModel(QObject *parent)
    : QObject{parent} {}

QGeoPositionInfo OneVehiclePositionModel::lastValue() const {
  if (m_data.count() > 0)
    return m_data.last()->value;
  return QGeoPositionInfo();
}

void OneVehiclePositionModel::insert(int timestamp, QGeoPositionInfo &value) {
  if (m_data.contains(timestamp))
    return;
  m_data[timestamp] = new PositionData{timestamp, value};
  emit dataChanged();

  if (m_data.lastKey() == timestamp)
    emit lastValueChanged();
}
