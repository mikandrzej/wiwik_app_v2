#include "app.h"
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>

App::App(MainWindow *mainWindow, QObject *parent)
    : QObject{parent}, m_mainWindow(mainWindow) {
  bindGui();

  connect(&m_dataProvider, &EgDataProvider::sensorLiveDataReceived,
          &m_carsMapModel, &EgCarsMapModel::onSensorDataReceived);

  connect(&m_dataProvider, &EgDataProvider::sensorLiveDataReceived,
          &m_vehiclesModel, &VehiclesModel::onSensorDataReceived);
  connect(&m_dataProvider, &EgDataProvider::vehiclesDataReceived,
          &m_vehiclesModel, &VehiclesModel::onVehiclesDataReceived);

  connect(&m_dataProvider, &EgDataProvider::restServerStateChanged, this,
          &App::onRestServerStateChanged);
}

void App::bindGui() {

  m_mainWindow->setVehModel(&m_vehiclesModel);

  connect(&m_dataProvider, &EgDataProvider::mqttServerStateChanged,
          m_mainWindow, &MainWindow::setMqttStatus);
  connect(&m_dataProvider, &EgDataProvider::restServerStateChanged,
          m_mainWindow, &MainWindow::setRestStatus);
  connect(&m_dataProvider, &EgDataProvider::sensorLiveDataReceived,
          m_mainWindow, &MainWindow::onSensorLiveDataReceived);
  connect(m_mainWindow->dialogAssignSensor(),
          &DialogAssignSensor::selectionFinished, &m_dataProvider,
          &EgDataProvider::assignVehicleToDevice);
  connect(m_mainWindow->pbAssignSensor(), &QPushButton::clicked, this,
          &App::onAssignSensorClicked);
  connect(m_mainWindow->pbEditVehicles(), &QPushButton::clicked, this,
          &App::onEditVehiclesClicked);
  connect(m_mainWindow, &MainWindow::vehicleHistoryDataRequested,
          &m_dataProvider, &EgDataProvider::requestVehicleHistoryData);
  connect(this, &App::historyDataReady, m_mainWindow,
          &MainWindow::onHistoryDataReady);
  connect(&m_dataProvider, &EgDataProvider::vehiclesHistoryDataReady,
          m_mainWindow, &MainWindow::onHistoryDataReady);
  connect(m_mainWindow->dialogEditVehicles(),
          &DialogEditVehicles::addNewVehicle, &m_dataProvider,
          &EgDataProvider::onAddNewVehicle);
}

void App::onSensorDataReceived(EgSensorData &sensorData) {
  qDebug() << "Sensor data received";
}

void App::onRestServerStateChanged(bool state) {
  if (state == true) {
    m_dataProvider.requestVehiclesData();
    m_dataProvider.requestDevicesData();
  }
}
void App::onAssignSensorClicked(bool checked) {
  m_mainWindow->dialogAssignSensor()->setVehiclesList(
      this->m_dataProvider.getVehiclesList());
  m_mainWindow->dialogAssignSensor()->setDevicesList(
      this->m_dataProvider.getDevicesList());
  m_mainWindow->dialogAssignSensor()->open();
}

void App::onEditVehiclesClicked(bool checked) {
  //  m_mainWindow->dialogEditVehicles()->setCarListModel(&m_carsModel);
  m_mainWindow->dialogEditVehicles()->open();
}
