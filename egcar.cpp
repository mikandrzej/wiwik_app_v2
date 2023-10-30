#include "egcar.h"

EgCar::EgCar(int id, QString title, QString plateNo, double temperature) {
  m_title = title;
  m_plateNo = plateNo;
  m_temperature = temperature;
}

QString EgCar::title() const { return m_title; }

void EgCar::setTitle(const QString &newTitle) {
  if (m_title != newTitle) {
    m_title = newTitle;

    emit titleChanged(newTitle);
  }
}

double EgCar::temperature() const { return m_temperature; }

void EgCar::setTemperature(double newTemperature) {
  if (qFuzzyCompare(m_temperature, newTemperature))
    return;
  m_temperature = newTemperature;
  emit temperatureChanged(newTemperature);
}

int EgCar::id() const { return m_id; }

void EgCar::setId(int newId) {
  if (m_id == newId)
    return;
  m_id = newId;
  emit idChanged(newId);
}

QString EgCar::plateNo() const { return m_plateNo; }

void EgCar::setPlateNo(const QString &newPlateNo) {
  if (m_plateNo == newPlateNo)
    return;
  m_plateNo = newPlateNo;
  emit plateNoChanged(newPlateNo);
}
