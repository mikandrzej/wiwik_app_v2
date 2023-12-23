#include "mapmodel.h"

MapModel mapHistoryModel;
MapModel mapLiveModel;

MapModel::MapModel(QObject* parent) : QAbstractListModel {parent} {}

bool MapModel::addMarker(int id)
{
    if (m_dataById.contains(id))
        return false;
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    auto data = new MapModelData(id, this);
    m_data.push_back(data);
    m_dataById.insert(id, data);
    endInsertRows();
    return true;
}

void MapModel::removeMarker(int id)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    auto idx = m_data.indexOf(data);
    beginRemoveRows(QModelIndex(), idx, idx);
    m_dataById.remove(id);
    m_data.remove(idx);
    endRemoveRows();
}

void MapModel::clearMarkers()
{
    beginResetModel();
    m_data.clear();
    m_dataById.clear();
    endResetModel();
}

void MapModel::setMarkerName(int id, const QString& name)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setName(name);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RoleName});
}

void MapModel::setMarkerColor(int id, const QColor& color)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setColor(color);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RoleMarkerColor});
}

void MapModel::setMarkerIcon(int id, const QIcon& icon)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setIcon(icon);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RoleMarkerIcon});
}

void MapModel::setMarkerCustomData(int id, QMap<QString, QString>& values)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setCustomData(values);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RoleCustomData});
}

void MapModel::setMarkerPathEnabled(int id, bool enabled)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setPathEnabled(enabled);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RolePathEnabled});
}

void MapModel::setMarkerPathColor(int id, const QColor& color)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setPathColor(color);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RolePathColor});
}

void MapModel::setMarkerPathWidth(int id, int width)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setPathWidth(width);
    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RolePathWidth});
}

void MapModel::updatePosition(int id, QGeoCoordinate& coordinate, bool update)
{
    if (!m_dataById.contains(id))
        return;
    auto* data = m_dataById[id];
    data->setPosition(coordinate);
    if (update)
    {
        auto idx = createIndex(m_data.indexOf(data), 0);
        emit dataChanged(idx, idx, {RoleMarkerPosition});
    }
}

void MapModel::setPath(int id, const QList<QGeoCoordinate>& path)
{
    QVariantList variantList;
    for (const auto& coord : path)
    {
        variantList.append(QVariant::fromValue(coord));
    }

    auto* data = m_dataById[id];
    data->setPath(variantList);

    auto idx = createIndex(m_data.indexOf(data), 0);
    emit dataChanged(idx, idx, {RolePath});
}

int MapModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    ;
    return m_data.count();
}

QVariant MapModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};
    if (index.row() >= m_data.length())
        return {};

    switch (role)
    {
        case RoleId:
            return m_data[index.row()]->id();
        case RoleName:
            return m_data[index.row()]->name();
        case RoleMarkerColor:
            return m_data[index.row()]->color();
        case RoleMarkerPosition:
            return QVariant::fromValue(m_data[index.row()]->position());
        case RoleMarkerIcon:
            return m_data[index.row()]->icon();
        case RolePath:
            return m_data[index.row()]->path();
        case RolePathEnabled:
            return m_data[index.row()]->pathEnabled();
        case RolePathColor:
            return m_data[index.row()]->pathColor();
        case RolePathWidth:
            return m_data[index.row()]->pathWidth();
        case RoleCustomData:
            return m_data[index.row()]->customDataString();
    }

    return {};
}

QHash<int, QByteArray> MapModel::roleNames() const
{
    const QHash<int, QByteArray> data = {
        {RoleId, "id"},
        {RoleName, "name"},
        {RoleMarkerPosition, "marker_position"},
        {RoleMarkerColor, "marker_color"},
        {RoleMarkerIcon, "marker_icon"},
        {RolePath, "path"},
        {RolePathEnabled, "path_enabled"},
        {RolePathWidth, "path_width"},
        {RolePathColor, "path_color"},
        {RoleCustomData, "custom_data"},
    };

    return data;
}

MapModelData::MapModelData(int id, QObject* parent) : QObject {parent}, m_id {id} {}

int MapModelData::id() const
{
    return m_id;
}

void MapModelData::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString MapModelData::name() const
{
    return m_name;
}

void MapModelData::setName(const QString& newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QColor MapModelData::color() const
{
    return m_color;
}

void MapModelData::setColor(const QColor& newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}

QIcon MapModelData::icon() const
{
    return m_icon;
}

void MapModelData::setIcon(const QIcon& newIcon)
{
    m_icon = newIcon;
    emit iconChanged();
}

QVariantList MapModelData::path() const
{
    return m_path;
}

void MapModelData::setPath(const QVariantList& newPath)
{
    if (m_path == newPath)
        return;
    m_path = newPath;
    emit pathChanged();
}

int MapModelData::pathWidth() const
{
    return m_pathWidth;
}

void MapModelData::setPathWidth(int newPathWidth)
{
    if (m_pathWidth == newPathWidth)
        return;
    m_pathWidth = newPathWidth;
    emit pathWidthChanged();
}

QColor MapModelData::pathColor() const
{
    return m_pathColor;
}

void MapModelData::setPathColor(const QColor& newPathColor)
{
    if (m_pathColor == newPathColor)
        return;
    m_pathColor = newPathColor;
    emit pathColorChanged();
}

QMap<QString, QString> MapModelData::customData() const
{
    return m_customData;
}

QString MapModelData::customDataString() const
{
    QString result;
    for (auto& key : m_customData.keys())
    {
        result += key;
        result += ": ";
        result += m_customData[key];
        result += "\r\n";
    }

    return result;
}

QGeoCoordinate MapModelData::position() const
{
    return m_position;
}

void MapModelData::setPosition(const QGeoCoordinate& newPosition)
{
    if (m_position == newPosition)
        return;
    m_position = newPosition;
    emit positionChanged();
}

bool MapModelData::pathEnabled() const
{
    return m_pathEnabled;
}

void MapModelData::setPathEnabled(bool newPathEnabled)
{
    if (m_pathEnabled == newPathEnabled)
        return;
    m_pathEnabled = newPathEnabled;
    emit pathEnabledChanged();
}

void MapModelData::setCustomData(const QMap<QString, QString>& newCustomData)
{
    if (m_customData == newCustomData)
        return;
    m_customData = newCustomData;
    emit customDataChanged();
}
