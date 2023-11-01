import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    Plugin
    {
        id: mapPlugin
        name: "osm"
    }
    Map
    {
        objectName: "map"
        id: map
        plugin: mapPlugin
        anchors.fill: parent
        activeMapType: MapType.CustomMap
        zoomLevel: 12
        minimumZoomLevel: 5
        maximumZoomLevel: 17


        MapItemView{
            model: app.carsModel
            delegate: MapPolyline{
                path: model.polylinePath
                line.width: model.polylineWidth
                line.color: model.polylineColor
            }
        }

        MapItemView{
            model: app.carsModel
            delegate: MapCircle{
                radius: model.circleRadius
                color:model.color
                border.width:model.circleBorderWidth
                center: QtPositioning.coordinate(model.markerPosition.latitude,
                                                 model.markerPosition.longitude)
                enabled: model.markerType === "circle"
            }
        }
    }
}
