#include "measure.h"
#include <utility>

Measure::Measure(QDateTime& timestamp, QVariant value, QObject* parent) : QObject {parent}, m_value(std::move(value)), m_timestamp(timestamp) {}

Measure::Measure(Measure* measure, QObject* parent) : QObject {parent}, m_value(measure->value()), m_timestamp(measure->timestamp()) {}

QVariant Measure::value() const
{
    return m_value;
}

QDateTime Measure::timestamp() const
{
    return m_timestamp;
}
