#ifndef DIALOGASSIGNSENSOR_H
#define DIALOGASSIGNSENSOR_H

#include "DataSource/EgDeviceData.h"
#include "DataSource/EgVehicleData.h"
#include <QDialog>
#include <QListWidgetItem>

namespace Ui
{
    class DialogAssignSensor;
}

class DialogAssignSensor : public QDialog
{
    Q_OBJECT

  public:
    explicit DialogAssignSensor(QWidget* parent = nullptr);
    ~DialogAssignSensor() override;

    void setVehiclesList(const EgVehiclesData& vehicleListData);
    void setDevicesList(const EgDevicesListData& deviceListData);

  signals:
    void selectionFinished(EgDeviceData* selectedDevice, EgVehicleData* selectedVehicle);

  private slots:

    void on_pb_clearSelectedVehicle_clicked();

    void on_DialogAssignSensor_finished(int result);

  private:
    Ui::DialogAssignSensor* ui;
    EgVehiclesData m_vehicleListData;
    EgDevicesListData m_deviceListData;
};

#endif   // DIALOGASSIGNSENSOR_H
