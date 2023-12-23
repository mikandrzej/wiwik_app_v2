#ifndef VEHICLEEDITMODEL_H
#define VEHICLEEDITMODEL_H

#include <QAbstractProxyModel>
#include <QObject>

class VehicleEditModel : public QAbstractProxyModel
{
    Q_OBJECT
  public:
    explicit VehicleEditModel(QObject* parent = nullptr);

    // QAbstractItemModel interface
  public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    // QAbstractProxyModel interface
  public:
    QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
};

#endif   // VEHICLEEDITMODEL_H
