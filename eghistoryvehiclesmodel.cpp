#include "eghistoryvehiclesmodel.h"

EgHistoryVehiclesModel::EgHistoryVehiclesModel(QObject* parent) : QIdentityProxyModel {parent} {}

QVariant EgHistoryVehiclesModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        QString vehName = sourceModel()->data(index, Qt::DisplayRole).toString();
        QString plateNo = sourceModel()->data(index.siblingAtColumn(1), Qt::DisplayRole).toString();
        return QString("%1 / %2").arg(plateNo, vehName);
    }
    return QVariant();
}
