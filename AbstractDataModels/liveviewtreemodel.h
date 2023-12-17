#ifndef LIVEVIEWTREEMODEL_H
#define LIVEVIEWTREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>


class TreeViewItem {
public:
    enum class Type {
        NotDefined,
        Vehicle,
        Device,
        Sensor
    };
private:
    Type m_type = TreeViewItem::Type::NotDefined;
    TreeViewItem *m_parent = nullptr;
    QList<TreeViewItem *> m_childs;
};

class LiveViewTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LiveViewTreeModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    TreeViewItem rootItem;
};

extern LiveViewTreeModel liveViewTreeModel;

#endif // LIVEVIEWTREEMODEL_H
