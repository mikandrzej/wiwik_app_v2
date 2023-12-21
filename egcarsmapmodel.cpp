#include "egcarsmapmodel.h"

EgCarsMapModel::EgCarsMapModel(QObject* parent) : QAbstractListModel {parent} {}

void EgCarsMapModel::addCar(EgCarMapModel* carModel)
{
    if (m_carModels.contains(carModel))
    {
        return;
    }
    int newIndex = m_carModels.count();
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_carModels.append(carModel);
    endInsertRows();
}

void EgCarsMapModel::removeCar(EgCarMapModel* carModel)
{
    if (m_carModels.contains(carModel))
    {
        int index = m_carModels.indexOf(carModel);
        beginRemoveRows(QModelIndex(), index, index);
        m_carModels.removeAt(index);
        endRemoveRows();
    }
}

void EgCarsMapModel::onSensorDataReceived(EgVehicleSensorData& sensorData)
{
    if (sensorData.dataType != EgSensorDataType::GpsPosition)
    {
        return;
    }
}

QHash<int, QByteArray> EgCarsMapModel::roleNames() const
{
    const QHash<int, QByteArray> roles = {
        {RoleId, "id"},
        {RoleName, "name"},
        {RolePolylineColor, "polylineColor"},
        {RolePolylineWidth, "polylineWidth"},
        {RoleCircleRadius, "circleRadius"},
        {RoleColor, "color"},
        {RoleCircleBorderWidth, "circleBorderWidth"},
        {RolePolylinePath, "polylinePath"},
        {RoleMarkerPosition, "markerPosition"},
        {RoleMarkerType, "markerType"},

    };

    return roles;
}

int EgCarsMapModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_carModels.count();
}

QVariant EgCarsMapModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (index.row() >= m_carModels.count())
    {
        return QVariant();
    }
    switch (role)
    {
        case RoleId:
            return m_carModels[index.row()]->id();
        case RoleName:
            return m_carModels[index.row()]->name();
        case RolePolylineColor:
            return m_carModels[index.row()]->polylineColor();
        case RolePolylineWidth:
            return m_carModels[index.row()]->polylineWidth();
        case RoleCircleRadius:
            return m_carModels[index.row()]->circleRadius();
        case RoleColor:
            return m_carModels[index.row()]->color();
        case RoleCircleBorderWidth:
            return m_carModels[index.row()]->circleBorderWidth();
        case RolePolylinePath:
            return m_carModels[index.row()]->polylinePath();
        case RoleMarkerPosition:
            return QVariant::fromValue(m_carModels[index.row()]->markerPosition());
        case RoleMarkerType:
            return m_carModels[index.row()]->markerType();
        default:
            return QVariant();
    }
}

QGeoCoordinate EgCarMapModel::markerPosition() const
{
    return m_markerPosition;
}

void EgCarMapModel::setMarkerPosition(const QGeoCoordinate& newMarkerPosition)
{
    if (m_markerPosition == newMarkerPosition)
        return;
    m_markerPosition = newMarkerPosition;
    emit markerPositionChanged();
}

QVariantList EgCarMapModel::polylinePath() const
{
    return m_polylinePath;
}

void EgCarMapModel::updatePosition(const QGeoCoordinate& newMarkerPosition)
{
    m_polylinePath.append(QVariant::fromValue(newMarkerPosition));
    setMarkerPosition(newMarkerPosition);
    emit polylinePathChanged();
}

void EgCarMapModel::clearPath()
{
    m_polylinePath.clear();
    emit polylinePathChanged();
}

QString EgCarMapModel::markerType() const
{
    const QHash<EgCarMapMarkerType, QString> mapper = {{EgCarMapMarkerType::Circle, "circle"}, {EgCarMapMarkerType::Icon, "icon"}, {EgCarMapMarkerType::Marker, "marker"}};
    return mapper.value(m_markerType);
}

void EgCarMapModel::setMarkerType(EgCarMapMarkerType newMarkerType)
{
    if (m_markerType == newMarkerType)
        return;
    m_markerType = newMarkerType;
    emit markerTypeChanged();
}

EgCarMapModel::EgCarMapModel(QObject* /*parent*/) {}

int EgCarMapModel::id() const
{
    return m_id;
}

void EgCarMapModel::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString EgCarMapModel::name() const
{
    return m_name;
}

void EgCarMapModel::setName(const QString& newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QColor EgCarMapModel::polylineColor() const
{
    return m_polylineColor;
}

void EgCarMapModel::setPolylineColor(const QColor& newPolylineColor)
{
    if (m_polylineColor == newPolylineColor)
        return;
    m_polylineColor = newPolylineColor;
    emit polylineColorChanged();
}

int EgCarMapModel::polylineWidth() const
{
    return m_polylineWidth;
}

void EgCarMapModel::setPolylineWidth(const int newPolylineWidth)
{
    if (m_polylineWidth == newPolylineWidth)
        return;
    m_polylineWidth = newPolylineWidth;
    emit polylineWidthChanged();
}

double EgCarMapModel::circleRadius() const
{
    return m_circleRadius;
}

void EgCarMapModel::setCircleRadius(double newCircleRadius)
{
    if (qFuzzyCompare(m_circleRadius, newCircleRadius))
        return;
    m_circleRadius = newCircleRadius;
    emit circleRadiusChanged();
}

QColor EgCarMapModel::color() const
{
    return m_color;
}

void EgCarMapModel::setColor(const QColor& newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}

int EgCarMapModel::circleBorderWidth() const
{
    return m_circleBorderWidth;
}

void EgCarMapModel::setCircleBorderWidth(int newCircleBorderWidth)
{
    if (m_circleBorderWidth == newCircleBorderWidth)
        return;
    m_circleBorderWidth = newCircleBorderWidth;
    emit circleBorderWidthChanged();
}
