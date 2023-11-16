#include "dialogeditvehicles.h"
#include "ui_dialogeditvehicles.h"
#include "vehicleseditmodel.h"
#include <QDebug>

DialogEditVehicles::DialogEditVehicles(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEditVehicles) {
  ui->setupUi(this);

  connect(ui->lv_vehicles, &QAbstractItemView::clicked, this,
          &DialogEditVehicles::onVehicleClicked);
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

void DialogEditVehicles::onVehicleClicked(const QModelIndex &index) {
  m_clickedIndex = index;
  ui->le_vehName->setText(
      m_carListModel
          ->data(index.siblingAtColumn(VehiclesEditModel::ColumnName),
                 Qt::DisplayRole)
          .toString());
  ui->le_plateNo->setText(
      m_carListModel
          ->data(index.siblingAtColumn(VehiclesEditModel::ColumnPlateNo),
                 Qt::DisplayRole)
          .toString());
}

void DialogEditVehicles::on_pb_saveVehicle_clicked() {
  if (!m_clickedIndex.isValid()) {
    qDebug() << "No vehicle selected";
    return;
  }
  bool ok;
  auto id =
      m_carListModel
          ->data(m_clickedIndex.siblingAtColumn(VehiclesEditModel::ColumnId),
                 Qt::DisplayRole)
          .toInt(&ok);
  if (!ok) {
    qDebug() << "Invalid vehicle index";
    return;
  }
  auto name = ui->le_vehName->text();
  auto plateNo = ui->le_plateNo->text();

  emit editVehicle(id, name, plateNo);
}
