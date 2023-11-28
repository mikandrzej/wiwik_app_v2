import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15
import Qt.labs.qmlmodels 1.0
import QtGraphicalEffects 1.15

Item {
    id: item
    signal mapPointClicked(vehicleId: int)

    Plugin
    {
        id: mapPlugin
        name: "mapboxgl"
    }

    Map
    {
        objectName: "map"
        id: map
        plugin: mapPlugin
        anchors.fill: parent
        activeMapType: MapType.CustomMap
        zoomLevel: 6
        minimumZoomLevel: 4
        maximumZoomLevel: 20
        center: QtPositioning.coordinate(52.37592865124644, 19.318247354764395)

        MouseArea {
            anchors.fill: parent
            onClicked: rectInfo.visible = false
        }
        GeocodeModel {
            id: geocodeModel
            plugin: map.plugin
            onStatusChanged: {
                if ((status == GeocodeModel.Ready) || (status == GeocodeModel.Error))
                    map.geocodeFinished()
            }
            onLocationsChanged:
            {
                if (count == 1) {
                    map.center.latitude = get(0).coordinate.latitude
                    map.center.longitude = get(0).coordinate.longitude
                }
            }
        }


        MapItemView{
            model: app.vehiclesModel
            delegate: MapPolyline{
                path: model.polylinePath
                line.width: map.zoomLevel / 5
                line.color: model.polylineColor
            }
        }

        MapItemView{
            model: app.vehiclesModel
            delegate:
                MapCircle{
                    radius: 2 * (17 - map.zoomLevel) * (17 - map.zoomLevel) * (17 - map.zoomLevel) + 5
                    color: model.color
                    border.width:model.circleBorderWidth
                    center: QtPositioning.coordinate(model.markerPosition.latitude,
                                                     model.markerPosition.longitude)
                    visible: model.markerType === "circle"
                }
        }

        MapItemView{
            model: app.vehiclesModel
            delegate:
                MapQuickItem{
                    property int vehicleId : model.id
                    coordinate: QtPositioning.coordinate(model.markerPosition.latitude,
                                                     model.markerPosition.longitude)
                    sourceItem: Image {
                        id: image
                        source: "map_pin.png"
                        width: 30
                        height: 30
                        layer.enabled: true
                        layer.effect: ShaderEffect {
                            property color targetColor: model.color
                            property variant source: sourceImage
                            fragmentShader: "
                                varying highp vec2 qt_TexCoord0;
                                uniform lowp sampler2D source;
                                uniform lowp vec4 targetColor;

                                void main() {
                                    lowp vec4 color = texture2D(source, qt_TexCoord0);

                                    // Zamień kolor biały (1, 1, 1) na przezroczysty
                                    if (color.r == 1.0 && color.g == 1.0 && color.b == 1.0) {
                                        discard;
                                    }

                                    // Zamień kolor czarny (0, 0, 0) na niestandardowy kolor
                                    if (color.rgb == vec3(0.0, 0.0, 0.0)) {
                                        color.rgb = targetColor.rgb;
                                    }

                                    gl_FragColor = color;
                                }
                            "
                        }

                    }
                    anchorPoint.x: 15
                    anchorPoint.y: 30
                    visible: model.markerType === "marker"
                    MouseArea {
                        cursorShape: "PointingHandCursor"
                        anchors.fill: parent
                        onClicked:
                            function() {
                                item.mapPointClicked(vehicleId)
                                console.log("map point clicked. Id:", vehicleId)
                            }

                    }
            }
        }
    }
    Rectangle
    {
        x: 20
        y: 20
        width: 200
        height: 100
        color: "blue"
        id: rectInfo
        visible: false
    }
}
