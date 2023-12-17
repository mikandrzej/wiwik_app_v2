#ifndef VEHICLELISTMODEL_H
#define VEHICLELISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

class VehicleListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit VehicleListModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    enum roles { RoleVehicleId = Qt::UserRole };

private:
};

extern VehicleListModel vehicleListModel;

#endif // VEHICLELISTMODEL_H
