#include "historyvehiclesproxymodel.h"
#include "vehiclesmodel.h"

HistoryVehiclesProxyModel::HistoryVehiclesProxyModel(QObject *parent) {}

QVariant HistoryVehiclesProxyModel::data(const QModelIndex &index,
                                         int role) const {
  if (index.isValid()) {
    if (role == Qt::DisplayRole) {
      QString id = sourceModel()
                       ->data(index.siblingAtColumn(VehiclesModel::ColumnId))
                       .toString();
      QString name =
          sourceModel()
              ->data(index.siblingAtColumn(VehiclesModel::ColumnName))
              .toString();
      QString plateNo =
          sourceModel()
              ->data(index.siblingAtColumn(VehiclesModel::ColumnPlateNo))
              .toString();
      return QString("%1: %2 / %3").arg(id, name, plateNo);
    }
  }
  return QSortFilterProxyModel::data(index, role);
}
