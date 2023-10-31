#ifndef EGVEHICLESMAP_H
#define EGVEHICLESMAP_H

#include <QStandardItemModel>
#include <QWidget>

class EgVehiclesMap : public QWidget {
  Q_OBJECT
public:
  explicit EgVehiclesMap(QWidget *parent = nullptr);

  QStandardItemModel *model() const;

signals:

private:
  QStandardItemModel *m_model;
  Q_PROPERTY(QObject *model READ model CONSTANT)
};

#endif // EGVEHICLESMAP_H
