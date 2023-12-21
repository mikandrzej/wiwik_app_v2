#include "formgpshistory.h"
#include "ui_formgpshistory.h"

#include "../AbstractDataModels/vehiclelistmodel.h"

#include <QDebug>

#include "../AbstractDataModels/mapmodel.h"
#include "../mapwidget.h"

FormGPSHistory::FormGPSHistory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormGPSHistory)
{
    ui->setupUi(this);

    //models
    ui->listView_vehicles->setModel(&vehicleListModel);

    // map
    auto *layout = new QVBoxLayout(ui->widget_map);
    m_mapHistoryWidget = new MapWidget(ui->widget_map);
    m_mapHistoryWidget->setModel(&mapHistoryModel);
    ui->widget_map->setLayout(layout);
    layout->addWidget(m_mapHistoryWidget);

    //date picker
    ui->dateEdit->setDate(QDate::currentDate());

    // connect(m_mapHistoryWidget,
    //         &EgVehiclesMap::mapMarkerClicked,
    //         this,
    //         &MainWindow::onHistoryMapMarkerClicked);
}

FormGPSHistory::~FormGPSHistory()
{
    delete ui;
}

void FormGPSHistory::onGpsDataReady(EgGpsListData &gpsList)
{
    mapHistoryModel.clearMarkers();

    if (gpsList.sensors.count() == 0)
        return;

    int currentId = 0;

    for (auto &sensor : gpsList.sensors) {
        auto &sensorName = sensor->sensor_name;
        auto &sensorAddress = sensor->sensor_address;

        mapHistoryModel.addMarker(currentId);

        mapHistoryModel.setMarkerName(currentId, sensorName + " " + sensorAddress);
        mapHistoryModel.setMarkerPathEnabled(currentId, true);
        mapHistoryModel.setMarkerPathWidth(currentId, 3);
        mapHistoryModel.setMarkerPathColor(currentId, QColorConstants::Blue);
        QList<QGeoCoordinate> path;
        for (auto &position : sensor->geolocations) {
            auto coordinate = position.coordinate();
            path.append(coordinate);
        }
        mapHistoryModel.setPath(currentId, path);
        mapHistoryModel.updatePosition(currentId, path.last());

        currentId++;
    }

    m_gpsList = gpsList;
    ui->horizontalSlider->setMinimum(0);
    auto sensorData = gpsList.sensors.first();
    ui->horizontalSlider->setMaximum(sensorData->timestamps.length());
}

void FormGPSHistory::on_listView_vehicles_clicked(const QModelIndex &index)
{
    repaintGpsData();
    qDebug() << "vehicle clicked with index " << index;

    qDebug() << "Current index: " << ui->listView_vehicles->currentIndex();
}

void FormGPSHistory::repaintGpsData()
{
    // obtain vehicle ID
    auto selectedIndex = ui->listView_vehicles->currentIndex();
    if (!selectedIndex.isValid())
        return;
    auto vehicleIdVariant = ui->listView_vehicles->model()
                                ->data(selectedIndex, VehicleListModel::roles::RoleVehicleId);
    if (!vehicleIdVariant.isValid())
        return;
    bool ok;
    auto vehicleId = vehicleIdVariant.toInt(&ok);
    if (!ok)
        return;

    //obtain selected date
    auto date = ui->dateEdit->date();

    emit requestGpsHistoryData(vehicleId, date);
}

void FormGPSHistory::on_dateEdit_userDateChanged(const QDate &date)
{
    repaintGpsData();
}

void FormGPSHistory::on_horizontalSlider_sliderMoved(int position) {}

void FormGPSHistory::on_horizontalSlider_valueChanged(int value)
{
    auto sensor = m_gpsList.sensors.first();
    if (sensor == nullptr)
        return;
    if (value >= sensor->geolocations.length())
        return;
    auto currentPosition = sensor->geolocations[value].coordinate();
    auto timestamp = sensor->timestamps[value];
    mapHistoryModel.updatePosition(0, currentPosition, true);
    ui->label_sliderDate->setText(timestamp.toString("hh:mm:ss"));
}
