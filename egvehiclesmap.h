#ifndef EGVEHICLESMAP_H
#define EGVEHICLESMAP_H

#include "egcarsmapmodel.h"

#include <QWidget>

class EgVehiclesMap : public QWidget {
  Q_OBJECT
public:
  explicit EgVehiclesMap(QWidget *parent = nullptr);

  QAbstractListModel *carsModel() const;

signals:

private:
  EgCarsMapModel *m_carsModel;
  Q_PROPERTY(QObject *carsModel READ carsModel CONSTANT)
};

#endif // EGVEHICLESMAP_H
