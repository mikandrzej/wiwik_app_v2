#include "vehicleseditmodel.h"
#include "vehiclesmodel.h"

VehiclesEditModel::VehiclesEditModel(QObject *parent)
    : QIdentityProxyModel{parent} {}

QVariant VehiclesEditModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    auto column = static_cast<TVehiclesEditModelColum>(index.column());
    switch (column) {
    case ColumnUserFriendly: {
      QString vehName = sourceModel()->data(index, Qt::DisplayRole).toString();
      QString plateNo =
          sourceModel()
              ->data(index.siblingAtColumn(VehiclesModel::ColumnName),
                     Qt::DisplayRole)
              .toString();
      return QString("%1 / %2").arg(plateNo, vehName);
    }
    case ColumnName:
      return sourceModel()->data(
          index.siblingAtColumn(VehiclesModel::ColumnName));
    case ColumnPlateNo:
      return sourceModel()->data(
          index.siblingAtColumn(VehiclesModel::ColumnPlateNo));
    case ColumnId:
      return sourceModel()->data(
          index.siblingAtColumn(VehiclesModel::ColumnId));
      break;
    }
  }
  return QVariant();
}
