#include "egvehiclesmap.h"
#include "egcarsmapmodel.h"

#include <QLayout>
#include <QQuickView>
#include <QtLocation>

EgVehiclesMap::EgVehiclesMap(QWidget *parent) : QWidget{parent} {

  m_carsModel = new EgCarsMapModel();

  auto quickView = new QQuickView();
  quickView->setResizeMode(QQuickView::SizeRootObjectToView);
  quickView->engine()->rootContext()->setContextProperty("app", this);
  quickView->setSource(QUrl(QString::fromUtf8(
      "file:///H:/Projects/C++/Qt/wiwik_app_widgets_5/geomap.qml")));

  QWidget *container = QWidget::createWindowContainer(quickView);
  container->setMinimumSize(quickView->size());
  container->setFocusPolicy(Qt::TabFocus);

  QVBoxLayout *layout = new QVBoxLayout(container);
  this->setLayout(layout);
  layout->addWidget(container);

  QObject *map = quickView->findChild<QObject *>("map");
  //  double zoom = map->property("zoomLevel").toDouble();
  //  qDebug() << "Quick view map zoom: " << zoom;

  //  for (int i = 0; i < 4; i++) {
  //    QStandardItem *item = new QStandardItem;
  //    item->setData(
  //        QVariant::fromValue(map->property("center").value<QGeoCoordinate>()),
  //        Qt::UserRole);
  //    item->setData(QVariant::fromValue(30), Qt::UserRole + 1);
  //    item->setData("red", Qt::UserRole + 2);
  //    item->setData(QVariant::fromValue(5), Qt::UserRole + 3);
  //    m_model->appendRow(item);
  //  }

  //  auto circle =
  //      new QGeoCircle(map->property("center").value<QGeoCoordinate>(), 5000);
  //  QMetaObject::invokeMethod(map, "addMapItem", Q_ARG(QGeoCircle *, circle));

  /*
  auto carModel = new EgCarMapModel();
  carModel->setColor(QColorConstants::Red);
  carModel->setPolylineColor(QColorConstants::Red);
  carModel->setPolylineWidth(2);
  carModel->setCircleBorderWidth(2);
  carModel->setCircleRadius(10);
  carModel->setMarkerType(EgCarMapMarkerType::Circle);
  auto center = map->property("center").value<QGeoCoordinate>();
  //  carModel->setMarkerPosition(center);
  carModel->updatePosition(center);
  auto longitude = center.longitude();
  auto latitude = center.latitude();
  longitude += 0.002;
  center.setLongitude(longitude);
  carModel->updatePosition(center);
  latitude += 0.2;
  center.setLatitude(latitude);
  carModel->updatePosition(center);

  m_carsModel->addCar(carModel);
*/
}

EgCarsMapModel *EgVehiclesMap::carsModel() const { return m_carsModel; }

void EgVehiclesMap::setCarsModel(EgCarsMapModel *model) { m_carsModel = model; }
