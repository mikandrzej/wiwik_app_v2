#ifndef TREEITEM_H
#define TREEITEM_H

#include "qfont.h"
#include <QList>
#include <QVariant>

struct TreeItemData {
  QVariant text;
  QFont *font;
  QVariant *decoration;
};

class TreeItem {
public:
  explicit TreeItem(const QList<TreeItemData *> &data, TreeItem *parentItem = 0,
                    unsigned int id = 0);
  ~TreeItem();

  void appendChild(TreeItem *child);

  TreeItem *child(int row);
  int childCount() const;
  int columnCount() const;
  TreeItemData *data(int column) const;
  int row() const;

  unsigned int getIndex() { return _id; };

  TreeItem *parentItem();

  void clear();

private:
  QList<TreeItem *> m_childItems;
  QList<TreeItemData *> m_itemData;
  TreeItem *m_parentItem;
  unsigned int _id;
};
Q_DECLARE_METATYPE(TreeItem *)

#endif // TREEITEM_H
