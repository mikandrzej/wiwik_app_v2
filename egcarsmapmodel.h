#ifndef EGCARSMAPMODEL_H
#define EGCARSMAPMODEL_H

#include "DataSource/EgVehicleData.h"
#include <QAbstractListModel>
#include <QColor>
#include <QGeoCoordinate>
#include <QObject>

enum EgCarMapMarkerType
{
    Circle,
    Icon,
    Marker
};

class EgCarMapModel : public QObject
{
    Q_OBJECT
  public:
    explicit EgCarMapModel(QObject* parent = nullptr);

    int id() const;
    void setId(int newId);

    QString name() const;
    void setName(const QString& newName);

    QColor polylineColor() const;
    void setPolylineColor(const QColor& newPolylineColor);

    int polylineWidth() const;
    void setPolylineWidth(const int newPolylineWidth);

    double circleRadius() const;
    void setCircleRadius(double newCircleRadius);

    QColor color() const;
    void setColor(const QColor& newColor);

    int circleBorderWidth() const;
    void setCircleBorderWidth(int newCircleBorderWidth);

    QGeoCoordinate markerPosition() const;
    void setMarkerPosition(const QGeoCoordinate& newMarkerPosition);

    QVariantList polylinePath() const;

    void updatePosition(const QGeoCoordinate& newMarkerPosition);
    void clearPath();

    QString markerType() const;
    void setMarkerType(EgCarMapMarkerType newMarkerType);

  signals:
    void markerPositionChanged();
    void nameChanged();
    void polylineColorChanged();
    void polylineWidthChanged();
    void circleRadiusChanged();
    void colorChanged();
    void circleBorderWidthChanged();

    void polylinePathChanged();

    void markerTypeChanged();

    void idChanged();

  private:
    int m_id;
    QString m_name;
    QColor m_polylineColor;
    int m_polylineWidth;
    double m_circleRadius;
    QColor m_color;
    int m_circleBorderWidth;
    QGeoCoordinate m_markerPosition;
    QVariantList m_polylinePath;
    EgCarMapMarkerType m_markerType;
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QColor polylineColor READ polylineColor NOTIFY polylineColorChanged)
    Q_PROPERTY(int polylineWidth READ polylineWidth NOTIFY polylineWidthChanged)
    Q_PROPERTY(double circleRadius READ circleRadius NOTIFY circleRadiusChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int circleBorderWidth READ circleBorderWidth NOTIFY circleBorderWidthChanged)
    Q_PROPERTY(QGeoCoordinate markerPosition READ markerPosition NOTIFY markerPositionChanged)
    Q_PROPERTY(QVariantList polylinePath READ polylinePath NOTIFY polylinePathChanged)
    Q_PROPERTY(QString markerType READ markerType NOTIFY markerTypeChanged)
};

class EgCarsMapModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    explicit EgCarsMapModel(QObject* parent = nullptr);

    void addCar(EgCarMapModel* carModel);
    void removeCar(EgCarMapModel* carModel);

  signals:
  public slots:
    void onSensorDataReceived(EgVehicleSensorData& sensorData);

  private:
    enum RoleNames
    {
        RoleId = Qt::UserRole,
        RoleName,
        RolePolylineColor,
        RolePolylineWidth,
        RoleCircleRadius,
        RoleColor,
        RoleCircleBorderWidth,
        RolePolylinePath,
        RoleMarkerPosition,
        RoleMarkerType,
    };

    QList<EgCarMapModel*> m_carModels;

    // QAbstractItemModel interface
  public:
    QHash<int, QByteArray> roleNames() const override;

    // QAbstractItemModel interface
  public:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
};

#endif   // EGCARSMAPMODEL_H
