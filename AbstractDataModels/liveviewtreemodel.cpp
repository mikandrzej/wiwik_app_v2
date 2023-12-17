#include "liveviewtreemodel.h"

#include "../DataSource/datacontainer.h"

LiveViewTreeModel liveViewTreeModel;

LiveViewTreeModel::LiveViewTreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    DataContainer::instance()->getVehicleList();
}

QModelIndex LiveViewTreeModel::index(int row, int column, const QModelIndex &parent) const
{
}

QModelIndex LiveViewTreeModel::parent(const QModelIndex &child) const
{
}

int LiveViewTreeModel::rowCount(const QModelIndex &parent) const
{
}

int LiveViewTreeModel::columnCount(const QModelIndex &parent) const
{
}

QVariant LiveViewTreeModel::data(const QModelIndex &index, int role) const
{
}
