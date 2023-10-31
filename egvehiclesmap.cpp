#include "egvehiclesmap.h"

#include <QLayout>
#include <QQuickView>
#include <QtLocation>

EgVehiclesMap::EgVehiclesMap(QWidget *parent) : QWidget{parent} {

  m_model = new QStandardItemModel(this);
  m_model->setItemRoleNames({{Qt::UserRole, "position"}});
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
  double zoom = map->property("zoomLevel").toDouble();
  qDebug() << "Quick view map zoom: " << zoom;
  for (int i = 0; i < 4; i++) {
    QStandardItem *item = new QStandardItem;
    item->setData(
        QVariant::fromValue(map->property("center").value<QGeoCoordinate>()),
        Qt::UserRole);
    m_model->appendRow(item);
  }

  //  auto circle =
  //      new QGeoCircle(map->property("center").value<QGeoCoordinate>(), 5000);
  //  QMetaObject::invokeMethod(map, "addMapItem", Q_ARG(QGeoCircle *, circle));
}

QStandardItemModel *EgVehiclesMap::model() const { return m_model; }
