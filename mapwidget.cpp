#include "mapwidget.h"
#include <QLayout>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

MapWidget::MapWidget(QWidget* parent) : QWidget {parent}
{
    m_quickView = new QQuickView();
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_quickView->setSource(QUrl(QString::fromUtf8("qrc:/mapWidget.qml")));

    QWidget* container = QWidget::createWindowContainer(m_quickView);
    container->setMinimumSize(m_quickView->size());
    container->setFocusPolicy(Qt::TabFocus);

    auto* layout = new QVBoxLayout(container);
    this->setLayout(layout);
    layout->addWidget(container);
}

QAbstractItemModel* MapWidget::model() const
{
    return m_model;
}

void MapWidget::setModel(QAbstractItemModel* newModel)
{
    if (m_model == newModel)
        return;
    m_model = newModel;
    m_quickView->engine()->rootContext()->setContextProperty("app", this);
    emit modelChanged();
}
