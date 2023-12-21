#include "colordelegate.h"

ColorDelegate::ColorDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *ColorDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QColorDialog *colorDialog = new QColorDialog(parent);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog);

    connect(colorDialog,
            &QColorDialog::currentColorChanged,
            this,
            &ColorDelegate::commitAndCloseEditor);

    return colorDialog;
}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QColorDialog *colorDialog = qobject_cast<QColorDialog *>(editor);
    if (colorDialog) {
        QColor color = index.model()->data(index, Qt::EditRole).value<QColor>();
        colorDialog->setCurrentColor(color);
    }
}

void ColorDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QColorDialog *colorDialog = qobject_cast<QColorDialog *>(editor);
    if (colorDialog) {
        model->setData(index, colorDialog->currentColor(), Qt::EditRole);
    }
}

void ColorDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QColor color = index.data(Qt::DisplayRole).value<QColor>();

    painter->fillRect(option.rect, color);
    painter->setPen(Qt::black);
    painter->drawRect(option.rect);
}

void ColorDelegate::updateEditorGeometry(QWidget *editor,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void ColorDelegate::commitAndCloseEditor()
{
    QColorDialog *editor = qobject_cast<QColorDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
