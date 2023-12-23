#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QMap>
#include <QObject>

class Device;
class Sensor;
class Vehicle;
class Measure;

class DataContainer : public QObject
{
    Q_OBJECT
  public:
    explicit DataContainer(QObject* parent = nullptr);

    void overwriteVehicles(QList<Vehicle*>& vehicles);
    void overwriteDevices(QList<Device*>& devices);
    void overwriteSensors(QList<Sensor*>& sensors);
    int getVehiclesCount();
    int getDevicesCount();
    int getSensorsCount();
    QList<Vehicle*> getVehicleList();
    QList<Device*> getDeviceList();
    QList<Sensor*> getSensorsList();

    Vehicle* getVehicleByIndex(int index);
    int getIndexByVehicle(Vehicle* vehicle);
    Vehicle* getVehicleById(int id);

    Device* getDeviceByIndex(int index);
    int getIndexByDevice(Device* device);
    Device* getDeviceById(int id);

    Sensor* getSensorByIndex(int index);
    int getIndexBySensor(Sensor* sensor);
    Sensor* getSensorById(int id);
    Sensor* getSensorByAddress(QString& address);

    void insertNewMeasure(QString& sensorAddress, Measure& measure);

    static DataContainer* instance();
  signals:
    void vehicleDataToBeInserted(int newIndex);
    void vehicleDataInsertDone(int newIndex);
    void vehicleDataChanged(int idx);

    void deviceDataToBeInserted(int newIndex);
    void deviceDataInsertDone(int newIndex);
    void deviceDataChanged(int idx);

    void sensorDataToBeInserted(int newIndex);
    void sensorDataInsertDone(int newIndex);
    void sensorDataChanged(int idx);

    void sensorsDataNeedsUpdate();
    void devicesDataNeedsUpdate();
    void vehiclesDataNeedsUpdate();

  private:
    QMap<int, Vehicle*> m_vehiclesMap;
    QList<Vehicle*> m_vehiclesList;
    void removeVehicle(int id);
    void addVehicle(Vehicle* vehicle);
    void updateVehicle(Vehicle* vehicle);

    QMap<int, Device*> m_devicesMap;
    QList<Device*> m_devicesList;
    void removeDevice(int id);
    void addDevice(Device* device);
    void updateDevice(Device* device);

    QMap<int, Sensor*> m_sensorsMap;
    QMap<QString, Sensor*> m_sensorsAddressMap;
    QList<Sensor*> m_sensorsList;
    void removeSensor(int id);
    void addSensor(Sensor* sensor);
    void updateSensor(Sensor* sensor);
};

#endif   // DATACONTAINER_H
