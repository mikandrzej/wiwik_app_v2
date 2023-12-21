#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QGeoCoordinate>
#include <QIcon>
#include <QObject>

class MapModelData : public QObject
{
    Q_OBJECT
  public:
    MapModelData(int id, QObject* parent = nullptr);

    int id() const;
    void setId(int newId);

    QString name() const;
    void setName(const QString& newName);

    QColor color() const;
    void setColor(const QColor& newColor);

    QIcon icon() const;
    void setIcon(const QIcon& newIcon);

    QVariantList path() const;
    void setPath(const QVariantList& newPath);

    bool pathEnabled() const;
    void setPathEnabled(bool newPathEnabled);

    int pathWidth() const;
    void setPathWidth(int newPathWidth);

    QColor pathColor() const;
    void setPathColor(const QColor& newPathColor);

    QMap<QString, QString> customData() const;
    void setCustomData(const QMap<QString, QString>& newCustomData);

    QString customDataString() const;

    QGeoCoordinate position() const;
    void setPosition(const QGeoCoordinate& newPosition);

  signals:
    void idChanged();

    void nameChanged();

    void colorChanged();

    void iconChanged();

    void pathChanged();

    void pathEnabledChanged();

    void pathWidthChanged();

    void pathColorChanged();

    void customDataChanged();

    void positionChanged();

  private:
    int m_id = 0;
    QString m_name;
    QColor m_color;
    QIcon m_icon;
    QGeoCoordinate m_position;
    QVariantList m_path;
    bool m_pathEnabled;
    int m_pathWidth;
    QColor m_pathColor;
    QMap<QString, QString> m_customData;
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged FINAL)
    Q_PROPERTY(QVariantList path READ path WRITE setPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(bool pathEnabled READ pathEnabled WRITE setPathEnabled NOTIFY pathEnabledChanged FINAL)
    Q_PROPERTY(int pathWidth READ pathWidth WRITE setPathWidth NOTIFY pathWidthChanged FINAL)
    Q_PROPERTY(QColor pathColor READ pathColor WRITE setPathColor NOTIFY pathColorChanged FINAL)
    Q_PROPERTY(QMap<QString, QString> customData READ customData WRITE setCustomData NOTIFY customDataChanged FINAL)
    Q_PROPERTY(QGeoCoordinate position READ position WRITE setPosition NOTIFY positionChanged FINAL)
};

class MapModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    explicit MapModel(QObject* parent = nullptr);

    bool addMarker(int id);
    void removeMarker(int id);
    void clearMarkers();
    void setMarkerName(int id, const QString& name);
    void setMarkerColor(int id, const QColor& color);
    void setMarkerIcon(int id, const QIcon& icon);
    void setMarkerCustomData(int id, QMap<QString, QString>& values);
    void setMarkerPathEnabled(int id, bool enabled);
    void setMarkerPathColor(int id, const QColor& color);
    void setMarkerPathWidth(int id, int width);
    void updatePosition(int id, QGeoCoordinate& coordinate, bool update = true);
    void setPath(int id, const QList<QGeoCoordinate>& path);

    // QAbstractItemModel interface
  public:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

  private:
    enum
    {
        RoleId = Qt::UserRole,
        RoleName,
        RoleMarkerPosition,
        RoleMarkerColor,
        RoleMarkerIcon,
        RolePath,
        RolePathEnabled,
        RolePathColor,
        RolePathWidth,
        RoleCustomData
    };

    QVector<MapModelData*> m_data;
    QMap<int, MapModelData*> m_dataById;
};

extern MapModel mapHistoryModel;
extern MapModel mapLiveModel;

#endif   // MAPMODEL_H
