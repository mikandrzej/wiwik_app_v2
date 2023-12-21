#include "onevehicletemperaturemodel.h"
#include <math.h>

OneVehicleTemperatureModel::OneVehicleTemperatureModel(QObject* parent) : QAbstractTableModel {parent} {}

double OneVehicleTemperatureModel::lastValue() const
{
    if (m_data.count() > 0)
        return m_data.last()->value;

    return NAN;
}

void OneVehicleTemperatureModel::insert(int timestamp, double value)
{
    int beforeElement = 0;
    int afterElement = m_data.count();
    int newIndex;

    if (m_data.count() == 0)
    {
        newIndex = 0;
    }
    else if (timestamp > m_data.last()->timestamp)
    {
        newIndex = m_data.count();
    }
    else if (timestamp < m_data[0]->timestamp)
    {
        newIndex = 0;
    }
    else
    {
        while (true)
        {
            int elements = afterElement - beforeElement;
            if (elements == 1)
            {
                newIndex = beforeElement + 1;
                if (m_data[beforeElement]->timestamp == timestamp)
                {
                    return;
                }
                break;
            }
            int checkElement = elements / 2 + beforeElement;

            int checkTimestamp = m_data[checkElement]->timestamp;
            if (timestamp > checkTimestamp)
            {
                beforeElement = checkElement;
            }
            else if (timestamp < checkTimestamp)
            {
                afterElement = checkElement;
            }
            else
            {
                // element already exists
                return;
            }
        }
    }

    auto newData = new TemperatureData {timestamp, value};
    m_data.insert(newIndex, newData);

    if (newIndex == m_data.count())
        emit lastValueChanged();
}

int OneVehicleTemperatureModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.count();
}

int OneVehicleTemperatureModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnMax;
}

QVariant OneVehicleTemperatureModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case ColumnTimestamp:
                    return m_data[index.row()]->timestamp;
                case ColumnTemperature:
                    return m_data[index.row()]->value;
            }
    }
    return QVariant();
}
