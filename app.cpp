#include "app.h"
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>

#include "DataSource/datacontainer.h"
#include "DataSource/egdataprovider.h"
#include "Forms/formgpshistory.h"

App::App(MainWindow *mainWindow, QObject *parent)
    : QObject{parent}, m_mainWindow(mainWindow) {
  bindGui();

  connect(&m_dataProvider,
          &EgDataProvider::sensorLiveDataReceived,
          &m_carsMapModel,
          &EgCarsMapModel::onSensorDataReceived);
  connect(&m_dataProvider,
          &EgDataProvider::sensorLiveDataReceived,
          &m_vehiclesModel,
          &VehiclesModel::onSensorDataReceived);

  connect(&m_dataProvider,
          &EgDataProvider::vehiclesDataReceived,
          &m_vehiclesModel,
          &VehiclesModel::onVehiclesDataReceived);
  connect(
      &m_dataProvider, &EgDataProvider::vehiclesDataReceived, this,
      [this](EgVehiclesData &) { this->m_dataProvider.requestDevicesData(); });

  connect(&m_dataProvider, &EgDataProvider::devicesDataReceived, this,
          [this](EgDevicesListData &) {
            this->m_dataProvider.requestSensorsData();
          });

  connect(&m_dataProvider, &EgDataProvider::restServerStateChanged, this,
          &App::onRestServerStateChanged);

  connect(DataContainer::instance(), &DataContainer::sensorsDataNeedsUpdate, &m_dataProvider, &EgDataProvider::requestSensorsData);

  //gps history
  connect(mainWindow->formGpsHistory(),
          &FormGPSHistory::requestGpsHistoryData,
          m_dataProvider.restDataSource(),
          &EgRestDataSource::requestGpsHistoryData);
  connect(m_dataProvider.restDataSource(),
          &EgRestDataSource::vehiclesGpsHistoryDataReady,
          mainWindow->formGpsHistory(),
          &FormGPSHistory::onGpsDataReady);

  //temperature history
  connect(mainWindow->formTemperatureHistory(),
          &FormTemperatureHistory::requestTemperatureHistoryData,
          m_dataProvider.restDataSource(),
          &EgRestDataSource::requestTemperatureHistoryData);
  connect(m_dataProvider.restDataSource(),
          &EgRestDataSource::vehiclesTemperatureHistoryDataReady,
          mainWindow->formTemperatureHistory(),
          &FormTemperatureHistory::onTemperatureDataReady);
}

void App::bindGui() {
    m_vehiclesEditModel.setSourceModel(&m_vehiclesModel);

    connect(&m_dataProvider,
            &EgDataProvider::mqttServerStateChanged,
            m_mainWindow,
            &MainWindow::setMqttStatus);
    connect(&m_dataProvider,
            &EgDataProvider::restServerStateChanged,
            m_mainWindow,
            &MainWindow::setRestStatus);
    connect(m_mainWindow->dialogAssignSensor(),
            &DialogAssignSensor::selectionFinished,
            &m_dataProvider,
            &EgDataProvider::assignVehicleToDevice);
    connect(m_mainWindow->pbAssignSensor(),
            &QPushButton::clicked,
            this,
            &App::onAssignSensorClicked);
    connect(m_mainWindow->pbEditVehicles(),
            &QPushButton::clicked,
            this,
            &App::onEditVehiclesClicked);
    connect(m_mainWindow,
            &MainWindow::vehicleHistoryDataRequested,
            &m_dataProvider,
            &EgDataProvider::requestVehicleHistoryData);
    connect(m_mainWindow->dialogEditVehicles(),
            &DialogEditVehicles::addNewVehicle,
            &m_dataProvider,
            &EgDataProvider::onAddNewVehicle);
    connect(m_mainWindow->dialogEditVehicles(),
            &DialogEditVehicles::editVehicle,
            &m_dataProvider,
            &EgDataProvider::onEditVehicle);

    connect(&m_dataProvider,
            &EgDataProvider::vehiclesDataReceived,
            &m_mainVehicleModel,
            [this](EgVehiclesData &vehiclesData) {
                for (auto &veh : vehiclesData.vehicles) {
                    auto data = new MainVehicleModelData();
                    data->setId(veh->id);
                    data->setName(veh->name);
                    data->setPlateNo(veh->plateNo);
                    m_mainVehicleModel.onNewVehicleData(data);
                }
            });
}


void App::onRestServerStateChanged(bool state) {
  if (state == true) {
    m_dataProvider.requestVehiclesData();
  }
}
void App::onAssignSensorClicked(bool /*checked*/) {
  m_mainWindow->dialogAssignSensor()->setVehiclesList(
      this->m_dataProvider.getVehiclesList());
  m_mainWindow->dialogAssignSensor()->setDevicesList(
      this->m_dataProvider.getDevicesList());
  m_mainWindow->dialogAssignSensor()->open();
}

void App::onEditVehiclesClicked(bool /*checked*/) {
  m_mainWindow->dialogEditVehicles()->setCarListModel(&m_vehiclesEditModel);
  m_mainWindow->dialogEditVehicles()->open();
}
