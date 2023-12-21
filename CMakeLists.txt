cmake_minimum_required(VERSION 3.5)

project(wiwik_app_widgets_5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif()

if(MSVC)
    add_compile_options(-W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wno-unused-function -Werror)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Quick Location)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt PrintSupport Quick Location)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        app.h app.cpp
        egsettings.h egsettings.cpp
        dialogassignsensor.h dialogassignsensor.cpp dialogassignsensor.ui
        eghistoryvehiclesmodel.h eghistoryvehiclesmodel.cpp
        dialogeditvehicles.h dialogeditvehicles.cpp dialogeditvehicles.ui
        egvehiclesmap.h egvehiclesmap.cpp

        egcarsmapmodel.h egcarsmapmodel.cpp
        vehiclesmodel.h vehiclesmodel.cpp
        onevehiclemodel.h onevehiclemodel.cpp
        onevehicletemperaturemodel.h onevehicletemperaturemodel.cpp
        onevehiclepositionmodel.h onevehiclepositionmodel.cpp
        onevehiclebatterymodel.h onevehiclebatterymodel.cpp
        livevehiclesproxymodel.h livevehiclesproxymodel.cpp
        historyvehiclesproxymodel.h historyvehiclesproxymodel.cpp
        chartwidget.h chartwidget.cpp
        vehicleseditmodel.h vehicleseditmodel.cpp

        treeitem.h treeitem.cpp
        resources.qrc
        vehicleeditmodel.h vehicleeditmodel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(wiwik_app_widgets_5
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET wiwik_app_widgets_5 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(wiwik_app_widgets_5 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(wiwik_app_widgets_5
            ${PROJECT_SOURCES}
            mapWidget.qml
            Forms/formgpshistory.h Forms/formgpshistory.cpp Forms/formgpshistory.ui
            Forms/formliveview.h Forms/formliveview.cpp Forms/formliveview.ui
            Forms/formtemperaturehistory.h Forms/formtemperaturehistory.cpp Forms/formtemperaturehistory.ui
        )
    endif()
endif()

add_library(CustomPlot SHARED
    QCustomPlot/qcustomplot.cpp QCustomPlot/qcustomplot.h
)
target_link_libraries(CustomPlot PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::PrintSupport)
target_compile_options(CustomPlot PRIVATE -Wno-deprecated-enum-enum-conversion -Wno-misleading-indentation)


add_library(Configuration STATIC
    Configuration/configuration.h Configuration/configuration.cpp
)
target_link_libraries(Configuration PRIVATE Qt${QT_VERSION_MAJOR}::Core)


add_library(DataModels STATIC
    DataModels/mainvehiclemodel.h DataModels/mainvehiclemodel.cpp
    DataModels/vehmodeltreeitem.h DataModels/vehmodeltreeitem.cpp

    DataModels/vehicle.h DataModels/vehicle.cpp
    DataModels/device.h DataModels/device.cpp
    DataModels/sensor.h DataModels/sensor.cpp
    DataModels/measure.h DataModels/measure.cpp
    DataModels/gpsdata.h DataModels/gpsdata.cpp


)
target_link_libraries(DataModels PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Location Configuration)


add_library(DataSource STATIC
    DataSource/egdataprovider.cpp DataSource/egdataprovider.h
    DataSource/egmqttdatasource.cpp DataSource/egmqttdatasource.h
    DataSource/egrestdatasource.cpp DataSource/egrestdatasource.h

    DataSource/EgVehicleData.h
    DataSource/EgDeviceData.h
    DataSource/datacontainer.h DataSource/datacontainer.cpp
)
target_link_libraries(DataSource PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Location Qt${QT_VERSION_MAJOR}::Mqtt DataModels Configuration)

add_library(AbstractDataModels STATIC
    AbstractDataModels/vehicletablemodel.h AbstractDataModels/vehicletablemodel.cpp
    AbstractDataModels/devicetablemodel.h AbstractDataModels/devicetablemodel.cpp
    AbstractDataModels/sensortablemodel.h AbstractDataModels/sensortablemodel.cpp

    AbstractDataModels/vehicletreemodel.h AbstractDataModels/vehicletreemodel.cpp

    AbstractDataModels/vehicletreenode.h AbstractDataModels/vehicletreenode.cpp
    AbstractDataModels/mapmodel.h AbstractDataModels/mapmodel.cpp
    AbstractDataModels/vehiclelistmodel.h AbstractDataModels/vehiclelistmodel.cpp
    AbstractDataModels/colordelegate.h AbstractDataModels/colordelegate.cpp

)
target_link_libraries(AbstractDataModels PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Location Qt${QT_VERSION_MAJOR}::Widgets DataSource)

add_library(Widgets STATIC
    mapwidget.h mapwidget.cpp

)
target_link_libraries(Widgets PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Quick)

target_link_libraries(wiwik_app_widgets_5 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Location
    CustomPlot
    DataModels
    DataSource
    AbstractDataModels
    Widgets
)

set_target_properties(wiwik_app_widgets_5 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS wiwik_app_widgets_5
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wiwik_app_widgets_5)
endif()
