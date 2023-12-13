#ifndef MAINDEVICEMODEL_H
#define MAINDEVICEMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class MainDeviceModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MainDeviceModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // MAINDEVICEMODEL_H
