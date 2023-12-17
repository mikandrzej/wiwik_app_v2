#ifndef VEHICLETREEMODEL_H
#define VEHICLETREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "vehicletreenode.h"

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
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  private:
  VehicleTreeNode *m_rootNode;

  QModelIndex indexForNode(VehicleTreeNode *node) const;
  VehicleTreeNode *nodeForIndex(const QModelIndex &index) const;
  int rowForNode(VehicleTreeNode *node) const;

  private slots:
      void onEndInsertRow();
      void onBeginInsertRow(QModelIndex &index);
      void onEndRemoveRow();
      void onBeginRemoveRow(QModelIndex &index);
};

extern VehicleTreeModel vehicleTreeModel;
#endif // VEHICLETREEMODEL_H
