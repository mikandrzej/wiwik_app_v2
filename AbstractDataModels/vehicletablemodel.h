#ifndef VEHICLETABLEMODEL_H
#define VEHICLETABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class VehicleTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit VehicleTableModel(QObject *parent = nullptr);

  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  enum Columns {
      ColumnId,
      ColumnName,
      ColumnPlateNo,
      ColumnColor,
      ColumnCommit,
      ColumnMax,
  };

  private:
  struct ColumnData
  {
      QString name;
  };
  QMap<int, ColumnData> m_columnData = {{ColumnId, {"id"}},
                                        {ColumnName, {"Nazwa"}},
                                        {ColumnPlateNo, {"Nr rej."}},
                                        {ColumnColor, {"Kolor"}},
                                        {ColumnCommit, {"Zapisz"}}};

  private slots:
  void onDataReset();
};

extern VehicleTableModel vehicleTableModel;
#endif // VEHICLETABLEMODEL_H
