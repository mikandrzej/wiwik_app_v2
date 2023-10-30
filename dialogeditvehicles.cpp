#include "dialogeditvehicles.h"
#include "ui_dialogeditvehicles.h"
#include <QDebug>

DialogEditVehicles::DialogEditVehicles(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEditVehicles) {
  ui->setupUi(this);
}

DialogEditVehicles::~DialogEditVehicles() { delete ui; }

void DialogEditVehicles::setCarListModel(QAbstractItemModel *newCarListModel) {
  m_carListModel = newCarListModel;

  ui->lv_vehicles->setModel(m_carListModel);
}

void DialogEditVehicles::on_pb_saveAsNewVehicle_clicked() {
  QString vehName = ui->le_vehName->text();
  QString plateNo = ui->le_plateNo->text();

  emit addNewVehicle(vehName, plateNo);
}
