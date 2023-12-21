#include "vehmodeltreeitem.h"

VehModelTreeItem::VehModelTreeItem(const MainModelData& data, VehModelTreeItem* parentItem) : m_itemData(data), m_parentItem(parentItem) {}

VehModelTreeItem::~VehModelTreeItem()
{
    qDeleteAll(m_childItems);
}

void VehModelTreeItem::appendChild(VehModelTreeItem* item)
{
    m_childItems.append(item);
}

VehModelTreeItem* VehModelTreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int VehModelTreeItem::childCount() const
{
    return m_childItems.count();
}

int VehModelTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<VehModelTreeItem*>(this));

    return 0;
}

const MainModelData& VehModelTreeItem::data() const
{
    return m_itemData;
}

VehModelTreeItem* VehModelTreeItem::parentItem()
{
    return m_parentItem;
}
