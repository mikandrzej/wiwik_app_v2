#ifndef EGCAR_H
#define EGCAR_H

#include <QObject>
#include <QStandardItem>
#include <QStandardItemModel>

class EgCar : QObject
{
    Q_OBJECT
  public:
    EgCar(int id = 0, QString title = "", QString plateNo = "", double temperature = 0.0);
    int id() const;
    QString title() const;
    QString plateNo() const;
    double temperature() const;

  public slots:
    void setId(int newId);
    void setTitle(const QString& newTitle);
    void setPlateNo(const QString& newPlateNo);
    void setTemperature(double newTemperature);

  signals:
    void idChanged(int newId);
    void titleChanged(const QString& title);
    void plateNoChanged(const QString& plateNo);
    void temperatureChanged(double temperature);

  private:
    int m_id;
    QString m_title;
    QString m_plateNo;
    double m_temperature;
};

#endif   // EGCAR_H
