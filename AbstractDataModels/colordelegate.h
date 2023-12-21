#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QColorDialog>
#include <QPainter>
#include <QStyledItemDelegate>

class ColorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ColorDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
public slots:
    void commitAndCloseEditor();
};

#endif // COLORDELEGATE_H
