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
            model: app.model
            delegate: MapCircle{
                id:uavPos
                radius: 5000
                color:'black'
                border.width:3
                center: QtPositioning.coordinate(model.position.latitude, model.position.longitude)
            }
        }
    }
}
