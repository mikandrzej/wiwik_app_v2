#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "egvehiclesmap.h"

#include <QCalendarWidget>
#include <QQuickView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  m_statusBarMqttLabel = new QLabel(ui->statusbar);
  m_statusBarMqttLabel->setAutoFillBackground(true);
  ui->statusbar->addPermanentWidget(m_statusBarMqttLabel);
  setMqttStatus(false);

  m_statusBarRestLabel = new QLabel(ui->statusbar);
  m_statusBarRestLabel->setAutoFillBackground(true);
  ui->statusbar->addPermanentWidget(m_statusBarRestLabel);
  setRestStatus(false);

  auto wg_chart = ui->wg_chart;
  //  m_historyPlot = new QCustomPlot(wg_chart);
  m_historyPlot = new EgTemperatureChart();
  auto historyPlotLayout = new QVBoxLayout(wg_chart);
  historyPlotLayout->addWidget(m_historyPlot->instance());

  m_historyPlot->instance()->setLocale(
      QLocale(QLocale::Polish, QLocale::Poland));

  m_dialogAssignSensor = new DialogAssignSensor(this);
  m_dialogEditVehicles = new DialogEditVehicles(this);

  connect(ui->tv_cars, &QTableView::clicked, this,
          &MainWindow::onLiveVehicleListClicked);
  connect(ui->lv_historyVehicles, &QListView::clicked, this,
          &MainWindow::onHistoryVehicleListClicked);

  connect(ui->pb_historyToday, &QPushButton::clicked, this,
          &MainWindow::onPbHistoryTodayClicked);
  connect(ui->de_history, &QDateEdit::userDateChanged, this,
          &MainWindow::onDeHistoryDateChanged);

  ui->de_history->setDate(QDate::currentDate());

  QVBoxLayout *layout = new QVBoxLayout(ui->w_map);
  m_mapWidget = new EgVehiclesMap(ui->w_map);
  ui->w_map->setLayout(layout);
  layout->addWidget(m_mapWidget);

  m_liveVehModel = new LiveVehiclesProxyModel(this);
  m_historyVehModel = new HistoryVehiclesProxyModel(this);
}

MainWindow::~MainWindow() { delete ui; }

EgTemperatureChart *MainWindow::historyPlot() { return m_historyPlot; }

void MainWindow::onPbHistoryTodayClicked(bool state) {
  ui->de_history->setDate(QDate::currentDate());
  plotHistoryData();
}

void MainWindow::onDeHistoryDateChanged(const QDate &date) {
  plotHistoryData();
}

DialogAssignSensor *MainWindow::dialogAssignSensor() const {
  return m_dialogAssignSensor;
}

QPushButton *MainWindow::pbAssignSensor() const { return ui->pb_assignSensor; }
QPushButton *MainWindow::pbEditVehicles() const { return ui->pb_editVehicles; }

void MainWindow::setVehModel(VehiclesModel *newVehModel) {
  m_vehModel = newVehModel;

  m_liveVehModel->setSourceModel(m_vehModel);
  ui->tv_cars->setModel(m_liveVehModel);
  m_historyVehModel->setSourceModel(newVehModel);
  ui->lv_historyVehicles->setModel(m_historyVehModel);

  connect(m_vehModel, &QAbstractItemModel::dataChanged, this,
          &MainWindow::onVehiclesModelDataChanged);
}

void MainWindow::setMqttStatus(bool status) {
  QString text = "MQTT state: ";
  QPalette palette = m_statusBarMqttLabel->palette();
  if (status) {
    palette.setColor(m_statusBarMqttLabel->backgroundRole(), Qt::green);
    text += "OK";
  } else {
    palette.setColor(m_statusBarMqttLabel->backgroundRole(), Qt::red);
    text += "ERROR";
  }
  m_statusBarMqttLabel->setPalette(palette);
  m_statusBarMqttLabel->setText(text);
}

void MainWindow::setRestStatus(bool status) {
  QString text = "REST state: ";
  QPalette palette = m_statusBarRestLabel->palette();
  if (status) {
    palette.setColor(m_statusBarRestLabel->backgroundRole(), Qt::green);
    text += "OK";
  } else {
    palette.setColor(m_statusBarRestLabel->backgroundRole(), Qt::red);
    text += "ERROR";
  }
  m_statusBarRestLabel->setPalette(palette);
  m_statusBarRestLabel->setText(text);
}

void MainWindow::onLiveVehicleListClicked(const QModelIndex &index) {
  ui->l_vehName->setText(
      m_vehModel
          ->data(index.siblingAtColumn(m_vehModel->ColumnName), Qt::DisplayRole)
          .toString());
  ui->l_plateNo->setText(
      m_vehModel
          ->data(index.siblingAtColumn(m_vehModel->ColumnPlateNo),
                 Qt::DisplayRole)
          .toString());
  ui->l_vehTemp->setText(
      m_vehModel
          ->data(index.siblingAtColumn(m_vehModel->ColumnTemperature),
                 Qt::DisplayRole)
          .toString());
}

void MainWindow::onHistoryVehicleListClicked(const QModelIndex &index) {
  m_historyVehicleListSelectedIndex = index;
  plotHistoryData();
}

void MainWindow::plotHistoryData() {
  if (!m_historyVehicleListSelectedIndex.isValid()) {
    return;
  }
  bool ok;
  int vehicleId =
      m_vehModel->data(m_historyVehicleListSelectedIndex, Qt::DisplayRole)
          .toInt(&ok);
  if (!ok)
    return;

  QDate date = ui->de_history->date();
  emit vehicleHistoryDataRequested(vehicleId, date);
}

void MainWindow::onHistoryDataReady(QVector<int> &timestamps,
                                    QVector<double> &temperatures) {
  QVector<double> x;
  x.reserve(timestamps.length());
  for (auto timestamp : timestamps) {
    x.append(timestamp);
  }
  m_historyPlot->clearGraph();

  m_historyPlot->setData(x, temperatures);
}

void MainWindow::historyDataAutoRescale() {
  if (ui->cb_autoReset->isChecked()) {
    m_historyPlot->resetView();
  }
}

void MainWindow::onSensorLiveDataReceived(EgSensorData &sensorData) {
  //  if (sensorData.dataType != EgSensorDataType::Temperature1) {
  //    return;
  //  }
  //  double value = sensorData.temperature;

  //  if (!m_historyVehicleListSelectedIndex.isValid()) {
  //    return;
  //  }
  //  auto vehicle =
  //      m_vehModel->getData().vehicles[m_historyVehicleListSelectedIndex.row()];
  //  if (sensorData.vehicleId != vehicle->id) {
  //    return;
  //  }

  //  auto date_a = sensorData.timestamp.date();
  //  auto date_b = ui->de_history->date();
  //  if (date_a != date_b) {
  //    return;
  //  }

  //  m_historyPlot->appendPoint(sensorData.timestamp.toSecsSinceEpoch(),
  //  value);

  //  historyDataAutoRescale();
}

void MainWindow::onVehiclesModelDataChanged(const QModelIndex &topLeft,
                                            const QModelIndex &bottomRight,
                                            const QVector<int> &roles) {
  // todo use slot/signals
  if (m_liveVehicleListSelectedRow >= topLeft.row() &&
      m_liveVehicleListSelectedRow <= bottomRight.row()) {
    if (roles.contains(Qt::DisplayRole)) {
      for (int col = topLeft.column(); col <= bottomRight.column(); col++) {
        QVariant data =
            m_vehModel->data(topLeft.sibling(m_liveVehicleListSelectedRow, col),
                             Qt::DisplayRole);
        switch (col) {
        case VehiclesModel::ColumnName:
          ui->l_vehName->setText(data.toString());
          break;
        case VehiclesModel::ColumnPlateNo:
          ui->l_plateNo->setText(data.toString());
          break;
        case VehiclesModel::ColumnTemperature:
          ui->l_vehTemp->setText(data.toString());
          break;
        case VehiclesModel::ColumnBattery:
          ui->l_sensorBatteryVoltage->setText(data.toString());
          break;
        }
      }
    }
  }
}

void MainWindow::on_pb_editVehicles_clicked() {}

DialogEditVehicles *MainWindow::dialogEditVehicles() const {
  return m_dialogEditVehicles;
}

void MainWindow::on_pb_historyReset_clicked() { m_historyPlot->resetView(); }

void MainWindow::on_pb_historyDateMinus_clicked() {
  ui->de_history->setDate(ui->de_history->date().addDays(-1));
}

void MainWindow::on_pb_historyDatePlus_clicked() {
  ui->de_history->setDate(ui->de_history->date().addDays(1));
}

void MainWindow::on_pb_historySelectDate_clicked() {
  auto dialog = new QDialog(this);
  auto calendar = new QCalendarWidget(dialog);
  dialog->resize(368, 205);
  calendar->setSelectedDate(ui->de_history->date());
  dialog->setModal(true);
  connect(calendar, &QCalendarWidget::selectionChanged, this,
          [calendar, this]() {
            this->ui->de_history->setDate(calendar->selectedDate());
          });
  connect(dialog, &QDialog::finished, this,
          [dialog, this](bool result) { delete dialog; });
  dialog->show();
}
