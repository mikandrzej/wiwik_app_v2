#include "dialogassignsensor.h"
#include "EgDeviceData.h"
#include "EgVehicleData.h"
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
    ui->lst_devices->addItem(device->id);
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
  auto selectedVehicles = ui->lst_vehicles->selectedItems();
  if (selectedDevices.empty()) {
    return;
  }
  EgDeviceData *selectedDevice = nullptr;

  for (int k = 0; k < m_deviceListData.devices.count(); k++) {
    auto dev = m_deviceListData.devices[k];
    if (selectedDevices[0]->text() == dev->id) {
      selectedDevice = dev;
      break;
    }
  }
  if (!selectedDevice) {
    return;
  }

  EgVehicleData *selectedVehicle = nullptr;
  if (!selectedVehicles.empty()) {
    for (int k = 0; k < m_vehicleListData.vehicles.count(); k++) {
      auto veh = m_vehicleListData.vehicles[k];
      if (selectedVehicles[0]->text() == veh->name) {
        selectedVehicle = veh;
        break;
      }
    }
  }

  emit selectionFinished(selectedDevice, selectedVehicle);
}
