#ifndef VEHICLETREEMODEL_H
#define VEHICLETREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class VehicleTreeModel : public QAbstractItemModel {
  Q_OBJECT
public:
  explicit VehicleTreeModel(QObject *parent = nullptr);

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
};

#endif // VEHICLETREEMODEL_H
