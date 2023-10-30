#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
  m_historyPlot = new QCustomPlot(wg_chart);
  auto historyPlotLayout = new QVBoxLayout(wg_chart);
  historyPlotLayout->addWidget(m_historyPlot);

  m_historyPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));

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
}

MainWindow::~MainWindow() { delete ui; }

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

void MainWindow::setVehModel(EgCarsModel *newVehModel) {
  m_vehModel = newVehModel;

  connect(m_vehModel, &EgCarsModel::dataChanged, this,
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

QCustomPlot *MainWindow::historyPlot() const { return m_historyPlot; }

void MainWindow::onLiveVehicleListClicked(const QModelIndex &index) {
  m_liveVehicleListSelectedRow = index.row();
  auto const vehicle_data = m_vehModel->getData().vehicles[index.row()];
  // todo use model
  ui->l_vehName->setText(vehicle_data->name);
  ui->l_plateNo->setText(vehicle_data->plateNo);
  ui->l_vehTemp->setText(QString("%1℃").arg(vehicle_data->temperature));
}

void MainWindow::onHistoryVehicleListClicked(const QModelIndex &index) {
  m_historyVehicleListSelectedIndex = index;
  plotHistoryData();
}

void MainWindow::plotHistoryData() {
  if (!m_historyVehicleListSelectedIndex.isValid()) {
    return;
  }
  int vehicleId = m_vehModel->getData()
                      .vehicles[m_historyVehicleListSelectedIndex.row()]
                      ->id;
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
  m_historyPlot->clearGraphs();
  m_historyPlot->addGraph();
  m_historyPlot->graph(0)->setPen(
      QPen(Qt::blue)); // line color blue for first graph
  m_historyPlot->graph(0)->setBrush(QBrush(QColor(
      0, 0, 255, 20))); // first graph will be filled with translucent blue
  m_historyPlot->xAxis2->setVisible(true);
  m_historyPlot->xAxis2->setTickLabels(false);
  m_historyPlot->yAxis2->setVisible(true);
  m_historyPlot->yAxis2->setTickLabels(false);

  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("hh:mm");
  m_historyPlot->xAxis->setTicker(dateTicker);

  connect(m_historyPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
          m_historyPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(m_historyPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),
          m_historyPlot->yAxis2, SLOT(setRange(QCPRange)));
  m_historyPlot->graph(0)->setData(x, temperatures);
  m_historyPlot->graph(0)->rescaleAxes();
  m_historyPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                 QCP::iSelectPlottables);
  m_historyPlot->replot();
}

void MainWindow::historyDataAutoRescale() {
  if (ui->cb_autoZoom->isChecked()) {
    m_historyPlot->graph(0)->rescaleAxes();
  }
}

void MainWindow::onSensorLiveDataReceived(EgSensorData &sensorData) {
  if (!m_historyVehicleListSelectedIndex.isValid()) {
    return;
  }

  auto vehicle =
      m_vehModel->getData().vehicles[m_historyVehicleListSelectedIndex.row()];
  if (sensorData.vehicleId != vehicle->id) {
    return;
  }
  auto date_a = sensorData.timestamp.date();
  auto date_b = ui->de_history->date();
  if (date_a != date_b) {
    return;
  }

  auto data = m_historyPlot->graph(0)->data();
  bool foundRange;
  auto range = data->keyRange(foundRange, QCP::sdPositive);
  if (!foundRange) {
    return;
  }
  auto sensorDataTimestamp = sensorData.timestamp.toSecsSinceEpoch();
  if ((double)sensorDataTimestamp <= range.upper) {
    return;
  }

  m_historyPlot->graph(0)->addData(sensorData.timestamp.toSecsSinceEpoch(),
                                   sensorData.temperature);
  m_historyPlot->replot();
  historyDataAutoRescale();
}

void MainWindow::onVehiclesModelDataChanged(const QModelIndex &topLeft,
                                            const QModelIndex &bottomRight,
                                            const QVector<int> &roles) {
  if (m_liveVehicleListSelectedRow >= topLeft.row() &&
      m_liveVehicleListSelectedRow <= bottomRight.row()) {
    if (roles.contains(Qt::DisplayRole)) {
      for (int col = topLeft.column(); col <= bottomRight.column(); col++) {
        switch (col) {
        case 0:
          ui->l_vehName->setText(m_vehModel->getData()
                                     .vehicles[m_liveVehicleListSelectedRow]
                                     ->name);
          break;
        case 1:
          ui->l_plateNo->setText(m_vehModel->getData()
                                     .vehicles[m_liveVehicleListSelectedRow]
                                     ->plateNo);
          break;
        case 2:
          ui->l_vehTemp->setText(
              QString("%1℃").arg(m_vehModel->getData()
                                     .vehicles[m_liveVehicleListSelectedRow]
                                     ->temperature));
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
