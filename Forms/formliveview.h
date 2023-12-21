#ifndef FORMLIVEVIEW_H
#define FORMLIVEVIEW_H

#include <QWidget>

class Sensor;
class MapWidget;

namespace Ui {
class FormLiveView;
}

class FormLiveView : public QWidget
{
    Q_OBJECT

public:
    explicit FormLiveView(QWidget *parent = nullptr);
    ~FormLiveView();

public slots:

private slots:
    void sensorPositionDataChanged(Sensor *sensor);

private:
    Ui::FormLiveView *ui;
    MapWidget *m_mapLiveWidget;
};

#endif // FORMLIVEVIEW_H
