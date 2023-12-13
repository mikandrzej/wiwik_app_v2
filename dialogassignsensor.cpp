#include "dialogassignsensor.h"
#include "DataSource/EgDeviceData.h"
#include "DataSource/EgVehicleData.h"
#include "ui_dialogassignsensor.h"

DialogAssignSensor::DialogAssignSensor(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogAssignSensor) {
  ui->setupUi(this);
}

DialogAssignSensor::~DialogAssignSensor() { delete ui; }

void DialogAssignSensor::setVehiclesList(
    const EgVehiclesData &vehicleListData) {
  m_vehicleListData = vehicleListData;
  ui->lst_vehicles->clear();
  foreach (auto vehicle, m_vehicleListData.vehicles) {
    ui->lst_vehicles->addItem(vehicle->name);
  }
}

void DialogAssignSensor::setDevicesList(
    const EgDevicesListData &deviceListData) {
  m_deviceListData = deviceListData;
  ui->lst_devices->clear();
  foreach (auto device, m_deviceListData.devices) {
    ui->lst_devices->addItem(device->name + " / " + device->serial_no);
  }
}

void DialogAssignSensor::on_pb_clearSelectedVehicle_clicked() {
  ui->lst_vehicles->clearSelection();
}

void DialogAssignSensor::on_DialogAssignSensor_finished(int result) {
  if (!result) {
    return;
  }

  auto selectedDevices = ui->lst_devices->selectedItems();
  if (selectedDevices.empty()) {
    return;
  }

  EgVehicleData *selectedVehicle = nullptr;
  auto selectedVehicles = ui->lst_vehicles->selectedItems();
  if (!selectedVehicles.empty()) {
    int selectedVehicleRow = ui->lst_vehicles->row(selectedVehicles.first());
    selectedVehicle = m_vehicleListData.vehicles[selectedVehicleRow];
  }

  EgDeviceData *selectedDevice = nullptr;
  int selectedDeviceRow = ui->lst_devices->row(selectedDevices.first());
  selectedDevice = m_deviceListData.devices[selectedDeviceRow];

  emit selectionFinished(selectedDevice, selectedVehicle);
}
