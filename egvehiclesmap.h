#ifndef EGVEHICLESMAP_H
#define EGVEHICLESMAP_H

#include "vehiclesmodel.h"

#include <QQuickView>
#include <QWidget>

class EgVehiclesMap : public QWidget {
  Q_OBJECT
public:
  explicit EgVehiclesMap(QWidget *parent = nullptr);

  VehiclesModel *vehiclesModel() const;
  void setVehiclesModel(VehiclesModel *model);

signals:
  void vehiclesModelChanged();
  void mapMarkerClicked(int vehicleId);

private:
  VehiclesModel *m_vehiclesModel;
  QQuickView *m_quickView;
  Q_PROPERTY(QObject *vehiclesModel READ vehiclesModel NOTIFY
                 vehiclesModelChanged)
};

#endif // EGVEHICLESMAP_H
