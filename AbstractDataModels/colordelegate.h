#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QColorDialog>
#include <QPainter>
#include <QStyledItemDelegate>

class ColorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
  public:
    explicit ColorDelegate(QObject* parent = nullptr);

    // QAbstractItemDelegate interface
  public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  public slots:
    void commitAndCloseEditor();
};

#endif   // COLORDELEGATE_H
