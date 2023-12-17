#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QAbstractItemModel>
#include <QObject>
#include <QQuickView>
#include <QWidget>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *newModel);

signals:
    void modelChanged();

private:
    QAbstractItemModel *m_model;
    QQuickView *m_quickView;
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged FINAL)
};

#endif // MAPWIDGET_H
