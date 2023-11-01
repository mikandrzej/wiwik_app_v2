cmake_minimum_required(VERSION 3.5)

project(wiwik_app_widgets_5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Quick Location)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt PrintSupport Quick Location)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        egcarsmodel.h egcarsmodel.cpp
        app.h app.cpp
        egsettings.h egsettings.cpp
        egdataprovider.h egdataprovider.cpp
        egmqttdatasource.h egmqttdatasource.cpp
        egrestdatasource.h egrestdatasource.cpp
        EgVehicleData.h
        dialogassignsensor.h dialogassignsensor.cpp dialogassignsensor.ui
        EgDeviceData.h
        eghistoryvehiclesmodel.h eghistoryvehiclesmodel.cpp
        dialogeditvehicles.h dialogeditvehicles.cpp dialogeditvehicles.ui
        egtemperaturechart.h egtemperaturechart.cpp
        egvehiclesmap.h egvehiclesmap.cpp
        geomap.qml
        egcarsmapmodel.h egcarsmapmodel.cpp
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
        )
    endif()
endif()

add_library(CustomPlot SHARED
    QCustomPlot/qcustomplot.cpp QCustomPlot/qcustomplot.h
)

target_link_libraries(CustomPlot PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::PrintSupport)


target_link_libraries(wiwik_app_widgets_5 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Mqtt Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Location CustomPlot)

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
