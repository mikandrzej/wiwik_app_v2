#ifndef VEHICLESEDITMODEL_H
#define VEHICLESEDITMODEL_H

#include <QIdentityProxyModel>

class VehiclesEditModel : public QIdentityProxyModel {
  Q_OBJECT
public:
  explicit VehiclesEditModel(QObject *parent = nullptr);
  enum TVehiclesEditModelColum {
    ColumnUserFriendly = 0,
    ColumnName,
    ColumnId,
    ColumnPlateNo
  };

  // QAbstractItemModel interface
public:
  QVariant data(const QModelIndex &index, int role) const override;
};

#endif // VEHICLESEDITMODEL_H
