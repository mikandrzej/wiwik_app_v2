#include "treeitem.h"
#include <QStringList>

TreeItem::TreeItem(const QList<TreeItemData*>& data, TreeItem* parent, unsigned int id)
{
    m_parentItem = parent;
    m_itemData = data;
    _id = id;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem* item)
{
    m_childItems.append(item);
}

TreeItem* TreeItem::child(int row)
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

TreeItemData* TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

TreeItem* TreeItem::parentItem()
{
    return m_parentItem;
}

void TreeItem::clear()
{
    m_childItems.clear();
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
