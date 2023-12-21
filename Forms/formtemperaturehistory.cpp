#include "formtemperaturehistory.h"
#include "../AbstractDataModels/vehiclelistmodel.h"
#include "../DataSource/EgDeviceData.h"
#include "../chartwidget.h"
#include "ui_formtemperaturehistory.h"

FormTemperatureHistory::FormTemperatureHistory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTemperatureHistory)
{
    ui->setupUi(this);

    auto layout = new QVBoxLayout(ui->wg_chart);
    m_chartWidget = new ChartWidget();
    layout->addWidget(m_chartWidget);

    ui->listView_vehicles->setModel(&vehicleListModel);

    //date picker
    ui->dateEdit->setDate(QDate::currentDate());

    // connect(ui->pb_historyToday, &QPushButton::clicked, this, &MainWindow::onPbHistoryTodayClicked);
    // connect(ui->de_history, &QDateEdit::userDateChanged, this, &MainWindow::onDeHistoryDateChanged);
}

FormTemperatureHistory::~FormTemperatureHistory()
{
    delete ui;
}

// void MainWindow::onPbHistoryTodayClicked(bool state) {
//     ui->de_history->setDate(QDate::currentDate());
//     plotHistoryData();
// }

// void MainWindow::historyDataAutoRescale() {
//     if (ui->cb_autoReset->isChecked()) {
//         m_historyPlot->rescaleAxis(Qt::XAxis);
//         m_historyPlot->rescaleAxis(Qt::YAxis);
//     }
// }

// void MainWindow::onSensorLiveDataReceived(EgVehicleSensorData &sensorData) {
//     if (sensorData.dataType == EgSensorDataType::Temperature) {
//         double timestamp = sensorData.timestamp.toSecsSinceEpoch();
//         double value = sensorData.temperature;

//         auto date_a = sensorData.timestamp.date();
//         auto date_b = ui->de_history->date();
//         if (date_a != date_b) {
//             return;
//         }

//         m_historyPlot->addData(timestamp, value, sensorData.sensorAddress);
//         historyDataAutoRescale();
//     } /*else if (sensorData.dataType == EgSensorDataType::GpsPosition) {
//         mapLiveModel.addMarker(sensorData.vehicleId);
//         auto coordinate = sensorData.geoPosition.coordinate();
//         mapLiveModel.updatePosition(sensorData.vehicleId, coordinate);
//     }*/

//     //    auto vehicle =
//     //        m_vehModel->getData().vehicles[m_historyVehicleListSelectedIndex.row()];
//     //    if (sensorData.vehicleId != vehicle->id) {
//     //      return;
//     //    }

//     //    auto date_a = sensorData.timestamp.date();
//     //    auto date_b = ui->de_history->date();
//     //    if (date_a != date_b) {
//     //      return;
//     //    }

//     //    m_historyPlot->appendPoint(sensorData.timestamp.toSecsSinceEpoch(),
//     //    value);

//     //    historyDataAutoRescale();
// }

// void MainWindow::on_pb_historyDateMinus_clicked()
// {
//     ui->de_history->setDate(ui->de_history->date().addDays(-1));
// }

// void MainWindow::on_pb_historyDatePlus_clicked()
// {
//     ui->de_history->setDate(ui->de_history->date().addDays(1));
// }

// void MainWindow::on_pb_historySelectDate_clicked()
// {
//     auto dialog = new QDialog(this);
//     auto calendar = new QCalendarWidget(dialog);
//     dialog->resize(368, 205);
//     calendar->setSelectedDate(ui->de_history->date());
//     dialog->setModal(true);
//     connect(calendar, &QCalendarWidget::selectionChanged, this, [calendar, this]() {
//         this->ui->de_history->setDate(calendar->selectedDate());
//     });
//     connect(dialog, &QDialog::finished, this, [dialog, this](bool result) { delete dialog; });
//     dialog->show();
// }

// void MainWindow::onHistoryDataReady(EgTemperatureListData &tempListData) {
//     m_historyPlot->clearGraphs();
//     for (auto &sensor_ref : tempListData.sensors.keys()) {
//         auto &sensor = tempListData.sensors[sensor_ref];

//         auto chart = m_historyPlot->addGraph(sensor->sensor_address,
//                                              sensor->sensor_name + " / " +
//                                                  sensor->sensor_address);
//         chart->setData(sensor->timestamps, sensor->values);
//     }
//     m_historyPlot->rescaleAxis(Qt::XAxis);
//     m_historyPlot->rescaleAxis(Qt::YAxis);

//     //  QVector<double> x;
//     //  x.reserve(timestamps.length());
//     //  for (auto timestamp : timestamps) {
//     //    x.append(timestamp);
//     //  }
//     //  m_historyPlot->clearGraphs();
//     //  auto chart = m_historyPlot->addGraph("unknown");
//     //  chart->setData(x, temperatures);
//     //  m_historyPlot->rescaleAxis(Qt::XAxis);
//     //  m_historyPlot->rescaleAxis(Qt::YAxis);
// }

// void MainWindow::onDeHistoryDateChanged(const QDate &date) {
//     plotHistoryData();
// }

// void MainWindow::on_pb_historyReset_clicked() {
//     m_historyPlot->rescaleAxis(Qt::XAxis);
//     m_historyPlot->rescaleAxis(Qt::YAxis);
// }

// void MainWindow::onTemperatureHistoryVehicleListClicked(const QModelIndex &index)
// {
//     m_historyVehicleListSelectedIndex = index;
//     plotHistoryData();
// }

void FormTemperatureHistory::on_listView_vehicles_clicked(const QModelIndex &index)
{
    m_selectedIndex = index;
    refreshTemperatureData();
}

void FormTemperatureHistory::refreshTemperatureData()
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

    auto date = ui->dateEdit->date();

    emit requestTemperatureHistoryData(vehicleId, date);
}

void FormTemperatureHistory::on_pb_historyToday_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
    refreshTemperatureData();
}

void FormTemperatureHistory::onTemperatureDataReady(EgTemperatureListData &temperatureData)
{
    m_chartWidget->clearGraphs();
    for (auto &sensor_ref : temperatureData.sensors.keys()) {
        auto &sensor = temperatureData.sensors[sensor_ref];

        auto chart = m_chartWidget->addGraph(sensor->sensor_address,
                                             sensor->sensor_name + " / " + sensor->sensor_address);
        chart->setData(sensor->timestamps, sensor->values);
    }
    m_chartWidget->rescaleAxis(Qt::XAxis);
    m_chartWidget->rescaleAxis(Qt::YAxis);

    //  QVector<double> x;
    //  x.reserve(timestamps.length());
    //  for (auto timestamp : timestamps) {
    //    x.append(timestamp);
    //  }
    //  m_historyPlot->clearGraphs();
    //  auto chart = m_historyPlot->addGraph("unknown");
    //  chart->setData(x, temperatures);
    //  m_historyPlot->rescaleAxis(Qt::XAxis);
    //  m_historyPlot->rescaleAxis(Qt::YAxis);
}
