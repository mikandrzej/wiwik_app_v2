#include "egvehiclesmap.h"

#include <QLayout>
#include <QQuickView>
#include <QtLocation>

EgVehiclesMap::EgVehiclesMap(QWidget *parent) : QWidget{parent} {

  m_quickView = new QQuickView();
  m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
  m_quickView->setSource(QUrl(QString::fromUtf8(
      "file:///H:/Projects/C++/Qt/wiwik_app_widgets_5/geomap.qml")));

  QWidget *container = QWidget::createWindowContainer(m_quickView);
  container->setMinimumSize(m_quickView->size());
  container->setFocusPolicy(Qt::TabFocus);

  QVBoxLayout *layout = new QVBoxLayout(container);
  this->setLayout(layout);
  layout->addWidget(container);

  QObject *map = m_quickView->findChild<QObject *>("map");

  connect(m_quickView->rootObject(), SIGNAL(mapPointClicked(int)), this,
          SIGNAL(mapMarkerClicked(int)));
}

VehiclesModel *EgVehiclesMap::vehiclesModel() const { return m_vehiclesModel; }

void EgVehiclesMap::setVehiclesModel(VehiclesModel *model) {
  m_vehiclesModel = model;
  m_quickView->engine()->rootContext()->setContextProperty("app", this);
  emit vehiclesModelChanged();
}
