#ifndef SENSORTABLEMODEL_H
#define SENSORTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class SensorTableModel : public QAbstractTableModel
{
    Q_OBJECT
  public:
    explicit SensorTableModel(QObject* parent = nullptr);

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
        ColumnName,
        ColumnAddress,
        ColumnType,
        ColumnDevice,
        ColumnLastMeasure,
        ColumnCommit,
        ColumnMax
    };
    struct ColumnData
    {
        QString name;
    };
    QMap<int, ColumnData> m_columnData = {
        {ColumnId, {"id"}},
        {ColumnName, {"Nazwa"}},
        {ColumnAddress, {"Adres"}},
        {ColumnType, {"Typ"}},
        {ColumnDevice, {"Urządzenie"}},
        {ColumnLastMeasure, {"Ostatni pomiar"}},
        {ColumnCommit, {"Zapisz"}},
    };

  private slots:
    void onDataReset();
};
extern SensorTableModel sensorTableModel;

#endif   // SENSORTABLEMODEL_H
