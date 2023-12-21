#include "formliveview.h"
#include "../AbstractDataModels/mapmodel.h"
#include "../AbstractDataModels/vehicletreemodel.h"
#include "../DataModels/gpsdata.h"
#include "../DataSource/datacontainer.h"
#include "../mapwidget.h"
#include "qgeopositioninfo.h"
#include "ui_formliveview.h"

FormLiveView::FormLiveView(QWidget* parent) : QWidget(parent), ui(new Ui::FormLiveView)
{
    ui->setupUi(this);

    // models
    ui->treeView_vehicles->setModel(&vehicleTreeModel);

    // map
    auto* layout = new QVBoxLayout(ui->widget_map);
    m_mapLiveWidget = new MapWidget(ui->widget_map);
    ui->widget_map->setLayout(layout);
    layout->addWidget(m_mapLiveWidget);
    // connect(m_mapHistoryWidget,
    //         &EgVehiclesMap::mapMarkerClicked,
    //         this,
    //         &MainWindow::onHistoryMapMarkerClicked);

    m_mapLiveWidget->setModel(&mapLiveModel);

    for (auto sensor : DataContainer::instance()->getSensorsList())
    {
        if (sensor->type() == "gps")
        {
            connect(sensor, &Sensor::lastMeasureChanged, this, [this, sensor]() { sensorPositionDataChanged(sensor); });
        }
    }

    connect(DataContainer::instance(), &DataContainer::sensorDataInsertDone, this, [this](int newIndex) {
        auto* newSensor = DataContainer::instance()->getSensorByIndex(newIndex);

        if (newSensor->type() == "gps")
        {
            connect(newSensor, &Sensor::lastMeasureChanged, this, [this, newSensor]() { sensorPositionDataChanged(newSensor); });
        }
    });
}

void FormLiveView::sensorPositionDataChanged(Sensor* sensor)
{
    auto sensorId = sensor->id();
    auto sensorName = sensor->name();
    auto sensorAddress = sensor->address();

    mapLiveModel.addMarker(sensorId);

    QColor color = Qt::black;
    try
    {
        auto device = sensor->device();
        if (device)
        {
            auto vehicle = device->vehicle();
            if (vehicle)
            {
                color = vehicle->color();
                mapLiveModel.setMarkerName(sensorId, vehicle->name());

                QMap<QString, QString> customData;
                for (auto* sensor : device->sensors())
                {
                    if (sensor->type() == "temperature")
                    {
                        customData["Temperatura"] = sensor->lastMeasureString();
                    }
                    if (sensor->type() == "battery")
                    {
                        customData["Bateria"] = sensor->lastMeasureString();
                    }
                }

                mapLiveModel.setMarkerCustomData(sensorId, customData);
            }
        }
    }
    catch (...)
    {
    };
    mapLiveModel.setMarkerColor(sensorId, color);
    mapLiveModel.setMarkerPathEnabled(sensorId, false);
    auto gpsData = sensor->lastMeasure()->value().value<GpsData>();
    auto coordinate = gpsData.coordinate();
    mapLiveModel.updatePosition(sensorId, coordinate);
}

FormLiveView::~FormLiveView()
{
    delete ui;
}
