#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QMap>
#include <QPen>
#include <QWidget>

class QCPGraph;
class QCustomPlot;
class GraphData;

class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget *parent = nullptr);
  GraphData *addGraph(QString name);

  void setAxisRange(Qt::Axis axis, QVariant min, QVariant max);

  void replot();
  void rescaleAxis(Qt::Axis axis);
  void setAxisTitle(Qt::Axis axis, QString title);
  void clearGraphs();

private:
  QCustomPlot *m_cp;
  QList<GraphData *> m_graphsData;
};

class GraphData {
public:
  explicit GraphData(ChartWidget *parent);
  void appendData(double x, double y, bool replot = true);
  void clearData();
  QCPGraph *graph;

  void setData(QVector<double> &x, QVector<double> &y, bool replot = true);

private:
  ChartWidget *parent;
};
#endif // CHARTWIDGET_H
