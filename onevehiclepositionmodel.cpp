#include "onevehiclepositionmodel.h"
#include <QVariant>

OneVehiclePositionModel::OneVehiclePositionModel(QObject* parent) : QObject {parent} {}

QGeoPositionInfo OneVehiclePositionModel::lastValue() const
{
    if (m_data.count() > 0)
    {
        auto value = m_data.last()->value;
        return value;
    }
    return {};
}

void OneVehiclePositionModel::insert(int timestamp, QGeoPositionInfo& value)
{
    if (m_data.contains(timestamp))
        return;
    m_data[timestamp] = new PositionData {timestamp, value};
    emit dataChanged();

    if (m_data.lastKey() == timestamp)
        emit lastValueChanged();
}

QVariantList OneVehiclePositionModel::path()
{
    QVariantList list;

    foreach (auto point, m_data.values())
    {
        list.append(QVariant::fromValue(point->value.coordinate()));
    }

    //    m_data.lowerBound() << wyszukiwanie najbliższego
    return list;
}
