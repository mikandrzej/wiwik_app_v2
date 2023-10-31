#ifndef EGTEMPERATURECHART_H
#define EGTEMPERATURECHART_H

#include <QWidget>

class EgTemperatureChart : public QWidget {
  Q_OBJECT
public:
  explicit EgTemperatureChart(QWidget *parent = nullptr);
  QWidget *instance();

  void clearGraph();
  void setData(QVector<double> &x, QVector<double> &y);
  void rescaleAxes();
  void appendPoint(double x, double y);
  void resetView();
signals:

private:
  QWidget *m_cp;
  QVector<double> m_x;
  QVector<double> m_y;
};

#endif // EGTEMPERATURECHART_H
