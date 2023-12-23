#ifndef LIVEVEHICLESPROXYMODEL_H
#define LIVEVEHICLESPROXYMODEL_H

#include <QSortFilterProxyModel>

class LiveVehiclesProxyModel : public QSortFilterProxyModel
{
  public:
    explicit LiveVehiclesProxyModel(QObject* parent = nullptr);
};

#endif   // LIVEVEHICLESPROXYMODEL_H
