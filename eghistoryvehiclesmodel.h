#ifndef EGHISTORYVEHICLESMODEL_H
#define EGHISTORYVEHICLESMODEL_H

#include <QIdentityProxyModel>
#include <QObject>

class EgHistoryVehiclesModel : public QIdentityProxyModel
{
    Q_OBJECT
  public:
    explicit EgHistoryVehiclesModel(QObject* parent = nullptr);

    // QAbstractProxyModel interface
  public:
    // QAbstractItemModel interface
  public:
    QVariant data(const QModelIndex& index, int role) const override;
};

#endif   // EGHISTORYVEHICLESMODEL_H
