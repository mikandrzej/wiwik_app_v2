#include "vehicletreemodel.h"

VehicleTreeModel vehicleTreeModel;

VehicleTreeModel::VehicleTreeModel(QObject* parent) : QAbstractItemModel {parent}
{
    m_rootNode = new VehicleTreeNode();
    connect(m_rootNode, &VehicleTreeNode::childToBeInserted, this, [this](VehicleTreeNode* node, int row) {
        auto index = indexForNode(node).siblingAtRow(row);
        this->onBeginInsertRow(index);
    });
    connect(m_rootNode, &VehicleTreeNode::childInsertDone, this, [this]() { onEndInsertRow(); });
    connect(m_rootNode, &VehicleTreeNode::childToBeRemoved, this, [this](VehicleTreeNode* node, int row) {
        auto index = indexForNode(node).siblingAtRow(row);
        this->onBeginRemoveRow(index);
    });
    connect(m_rootNode, &VehicleTreeNode::childRemovingDone, this, [this]() { onEndRemoveRow(); });
    connect(m_rootNode, &VehicleTreeNode::dataChanged, this, [this](VehicleTreeNode* node) {
        auto index = indexForNode(node);
        emit this->dataChanged(index, index.siblingAtColumn(rowCount(index.parent())));
    });
}

QModelIndex VehicleTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (hasIndex(row, column, parent))
    {
        auto* parentNode = nodeForIndex(parent);
        auto* childNode = parentNode->childs().at(row);
        return createIndex(row, column, childNode);
    }
    return {};
}

QModelIndex VehicleTreeModel::parent(const QModelIndex& child) const
{
    auto* childNode = nodeForIndex(child);
    auto* parentNode = childNode->parent();
    if (parentNode == m_rootNode)
    {
        return {};
    }
    int row = rowForNode(parentNode);
    if (row < 0)
        return {};
    int column = 0;
    return createIndex(row, column, parentNode);
}

int VehicleTreeModel::rowCount(const QModelIndex& parent) const
{
    auto* parentNode = nodeForIndex(parent);
    auto childCount = parentNode->childCount();
    return childCount;
}

int VehicleTreeModel::columnCount(const QModelIndex& parent) const
{
    auto* parentNode = nodeForIndex(parent);
    return parentNode->columnCount();
}

QVariant VehicleTreeModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        auto* node = nodeForIndex(index);
        return node->data(index.column(), role);
    }
    return {};
}

QModelIndex VehicleTreeModel::indexForNode(VehicleTreeNode* node) const
{
    if (node == m_rootNode)
        return {};
    int row = rowForNode(node);
    int column = 0;
    return createIndex(row, column, node);
}

VehicleTreeNode* VehicleTreeModel::nodeForIndex(const QModelIndex& index) const
{
    if (index.isValid())
    {
        return static_cast<VehicleTreeNode*>(index.internalPointer());
    }
    return m_rootNode;
}

int VehicleTreeModel::rowForNode(VehicleTreeNode* node) const
{
    if (node == nullptr)
        return -1;
    if (node->parent() == nullptr)
        return -1;
    return node->parent()->childs().indexOf(node);
}

void VehicleTreeModel::onEndInsertRow()
{
    endInsertRows();
}

void VehicleTreeModel::onBeginInsertRow(QModelIndex& index)
{
    beginInsertRows(index, index.row(), index.row());
}

void VehicleTreeModel::onEndRemoveRow()
{
    endRemoveRows();
}

void VehicleTreeModel::onBeginRemoveRow(QModelIndex& index)
{
    beginRemoveRows(index, index.row(), index.row());
}

QVariant VehicleTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        switch (role)
        {
            case Qt::DisplayRole:
                switch (section)
                {
                    case 0:
                        return "Nazwa";
                        break;
                    case 1:
                        return "Dane";
                        break;
                }
                break;
        }
    }

    return {};
}
