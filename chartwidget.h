#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QMap>
#include <QPen>
#include <QWidget>

class QCPGraph;
class QCustomPlot;
class GraphData;

class ChartWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit ChartWidget(QWidget* parent = nullptr);
    GraphData* addGraph(QString& sensor_addr, const QString& name);

    void setAxisRange(Qt::Axis axis, const QVariant& min, const QVariant& max);

    void replot();
    void rescaleAxis(Qt::Axis axis);
    void setAxisTitle(Qt::Axis axis, const QString& title);
    void clearGraphs();

    void addData(double x, double y, const QString& chart_ref);

  private:
    QCustomPlot* m_cp;
    QList<GraphData*> m_graphsData;
};

class GraphData
{
  public:
    explicit GraphData(ChartWidget* parent, QString& chartRef);
    void appendData(double x, double y, bool replot = true);
    void clearData();
    QCPGraph* graph;

    void setData(QVector<double>& x, QVector<double>& y, bool replot = true);

    QString reference() const;

  private:
    ChartWidget* parent;
    QString m_reference;
};
#endif   // CHARTWIDGET_H
