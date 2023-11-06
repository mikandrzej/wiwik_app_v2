#ifndef ONEVEHICLETEMPERATUREMODEL_H
#define ONEVEHICLETEMPERATUREMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class OneVehicleTemperatureModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit OneVehicleTemperatureModel(QObject *parent = nullptr);

  double lastValue() const;
  enum ColumnTypes { ColumnTimestamp, ColumnTemperature, ColumnMax };

public slots:
  void insert(int timestamp, double value);
signals:

  void lastValueChanged();

private:
  struct TemperatureData {
    int timestamp;
    double value;
  };
  QList<TemperatureData *> m_data;
  Q_PROPERTY(double lastValue READ lastValue NOTIFY lastValueChanged)

  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
};

#endif // ONEVEHICLETEMPERATUREMODEL_H
