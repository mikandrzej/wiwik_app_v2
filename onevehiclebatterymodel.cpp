#include "onevehiclebatterymodel.h"
#include <math.h>

OneVehicleBatteryModel::OneVehicleBatteryModel(QObject *parent)
    : QObject{parent} {}

double OneVehicleBatteryModel::lastValue() const {
  if (m_data.count() > 0)
    return m_data.last()->value;

  return NAN;
}

void OneVehicleBatteryModel::insert(int timestamp, double value) {
  if (m_data.contains(timestamp))
    return;
  m_data[timestamp] = new BatteryData{timestamp, value};
  emit dataChanged();

  if (m_data.lastKey() == timestamp)
    emit lastValueChanged();
}
