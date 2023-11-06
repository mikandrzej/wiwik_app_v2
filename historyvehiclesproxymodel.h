#ifndef HISTORYVEHICLESPROXYMODEL_H
#define HISTORYVEHICLESPROXYMODEL_H

#include <QSortFilterProxyModel>

class HistoryVehiclesProxyModel : public QSortFilterProxyModel {
public:
  explicit HistoryVehiclesProxyModel(QObject *parent = nullptr);

  // QAbstractItemModel interface
public:
  QVariant data(const QModelIndex &index, int role) const override;
};

#endif // HISTORYVEHICLESPROXYMODEL_H
