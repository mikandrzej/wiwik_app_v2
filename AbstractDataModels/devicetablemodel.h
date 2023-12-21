#ifndef DEVICETABLEMODEL_H
#define DEVICETABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class DeviceTableModel : public QAbstractTableModel
{
    Q_OBJECT
  public:
    explicit DeviceTableModel(QObject* parent = nullptr);

    // QAbstractItemModel interface
  public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

  private:
    enum Columns
    {
        ColumnId,
        ColumnSerialNo,
        ColumnName,
        ColumnVehicle,
        ColumnType,
        ColumnCommit,
        ColumnMax
    };
    struct ColumnData
    {
        QString name;
    };
    QMap<int, ColumnData> m_columnData = {
        {ColumnId, {"id"}},
        {ColumnSerialNo, {"Numer ser."}},
        {ColumnName, {"Nazwa"}},
        {ColumnVehicle, {"Pojazd"}},
        {ColumnType, {"Typ"}},
        {ColumnCommit, {"Zapisz"}},
    };

  private slots:
    void onDataReset();
};

extern DeviceTableModel deviceTableModel;
#endif   // DEVICETABLEMODEL_H
