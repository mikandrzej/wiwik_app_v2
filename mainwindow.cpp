#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AbstractDataModels/colordelegate.h"
#include "AbstractDataModels/devicetablemodel.h"
#include "AbstractDataModels/mapmodel.h"
#include "AbstractDataModels/sensortablemodel.h"
#include "AbstractDataModels/vehicletablemodel.h"
#include "DataModels/mainvehiclemodel.h"
#include "chartwidget.h"
#include "egvehiclesmap.h"
#include "qerrormessage.h"
#include <QCalendarWidget>
#include <QQuickView>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // include external ui's
    m_formGpsHistory = new FormGPSHistory(this);
    ui->tab_gpsHistory->layout()->addWidget(m_formGpsHistory);
    //
    m_formLiveView = new FormLiveView(this);
    ui->tab_liveView->layout()->addWidget(m_formLiveView);
    //
    m_formTemperatureHistory = new FormTemperatureHistory(this);
    ui->tab_temperatureHistory->layout()->addWidget(m_formTemperatureHistory);

    //
    m_statusBarMqttLabel = new QLabel(ui->statusbar);
    m_statusBarMqttLabel->setAutoFillBackground(true);
    ui->statusbar->addPermanentWidget(m_statusBarMqttLabel);
    setMqttStatus(false);

    m_statusBarRestLabel = new QLabel(ui->statusbar);
    m_statusBarRestLabel->setAutoFillBackground(true);
    ui->statusbar->addPermanentWidget(m_statusBarRestLabel);
    setRestStatus(false);

    m_dialogAssignSensor = new DialogAssignSensor(this);
    m_dialogEditVehicles = new DialogEditVehicles(this);

    ui->tv_vehicleConfig->setModel(&vehicleTableModel);
    ui->tv_DeviceConfig->setModel(&deviceTableModel);
    ui->tv_SensorConfig->setModel(&sensorTableModel);

    static ColorDelegate colorDelegate;
    ui->tv_vehicleConfig->setItemDelegateForColumn(VehicleTableModel::ColumnColor, &colorDelegate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

DialogAssignSensor* MainWindow::dialogAssignSensor() const
{
    return m_dialogAssignSensor;
}

QPushButton* MainWindow::pbAssignSensor() const
{
    return ui->pb_assignSensor;
}
QPushButton* MainWindow::pbEditVehicles() const
{
    return ui->pb_editVehicles;
}

void MainWindow::setMqttStatus(bool status)
{
    QString text = "MQTT state: ";
    QPalette palette = m_statusBarMqttLabel->palette();
    if (status)
    {
        palette.setColor(m_statusBarMqttLabel->backgroundRole(), Qt::green);
        text += "OK";
    }
    else
    {
        palette.setColor(m_statusBarMqttLabel->backgroundRole(), Qt::red);
        text += "ERROR";
    }
    m_statusBarMqttLabel->setPalette(palette);
    m_statusBarMqttLabel->setText(text);
}

void MainWindow::setRestStatus(bool status)
{
    QString text = "REST state: ";
    QPalette palette = m_statusBarRestLabel->palette();
    if (status)
    {
        palette.setColor(m_statusBarRestLabel->backgroundRole(), Qt::green);
        text += "OK";
    }
    else
    {
        palette.setColor(m_statusBarRestLabel->backgroundRole(), Qt::red);
        text += "ERROR";
    }
    m_statusBarRestLabel->setPalette(palette);
    m_statusBarRestLabel->setText(text);
}

void MainWindow::onGpsHistoryVehicleListClicked(const QModelIndex& /*index*/) {}

FormTemperatureHistory* MainWindow::formTemperatureHistory() const
{
    return m_formTemperatureHistory;
}

FormLiveView* MainWindow::formLiveView() const
{
    return m_formLiveView;
}

FormGPSHistory* MainWindow::formGpsHistory() const
{
    return m_formGpsHistory;
}
void MainWindow::on_pb_editVehicles_clicked() {}

DialogEditVehicles* MainWindow::dialogEditVehicles() const
{
    return m_dialogEditVehicles;
}
