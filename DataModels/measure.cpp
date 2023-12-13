#include "measure.h"

Measure::Measure(QDateTime &timestamp, QVariant value, QObject *parent)
    : m_value(value), m_timestamp(timestamp), QObject{parent} {}
Measure::Measure(Measure *measure, QObject *parent)
    : m_value(measure->value()), m_timestamp(measure->timestamp()),
      QObject{parent} {}

QVariant Measure::value() const { return m_value; }

QDateTime Measure::timestamp() const { return m_timestamp; }
