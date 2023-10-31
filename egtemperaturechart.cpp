#include "egtemperaturechart.h"
#include "QCustomPlot/qcustomplot.h"

EgTemperatureChart::EgTemperatureChart(QWidget *parent) : QWidget{parent} {
  m_cp = new QCustomPlot(parent);
}

QWidget *EgTemperatureChart::instance() { return m_cp; }

void EgTemperatureChart::clearGraph() {
  auto cp = (QCustomPlot *)m_cp;
  cp->clearGraphs();
  cp->addGraph();
  cp->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
  cp->graph(0)->setBrush(QBrush(QColor(
      0, 0, 255, 20))); // first graph will be filled with translucent blue
  cp->xAxis2->setVisible(true);
  cp->xAxis2->setTickLabels(false);
  cp->yAxis2->setVisible(true);
  cp->yAxis2->setTickLabels(false);

  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("hh:mm");
  cp->xAxis->setTicker(dateTicker);

  connect(cp->xAxis, SIGNAL(rangeChanged(QCPRange)), cp->xAxis2,
          SLOT(setRange(QCPRange)));
  connect(cp->yAxis, SIGNAL(rangeChanged(QCPRange)), cp->yAxis2,
          SLOT(setRange(QCPRange)));

  cp->addGraph();
  cp->graph(1)->setPen(QPen(Qt::red));
  cp->graph(1)->setData(QVector<double>({0, 1e+300}),
                        QVector<double>({-10, -10}));
  cp->addGraph();
  cp->graph(2)->setPen(QPen(Qt::green));
  cp->graph(2)->setData(QVector<double>({0, 1e+300}),
                        QVector<double>({10, 10}));

  cp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                      QCP::iSelectPlottables);
  resetView();
}

void EgTemperatureChart::setData(QVector<double> &x, QVector<double> &y) {
  auto cp = (QCustomPlot *)m_cp;
  m_x = x;
  m_y = y;
  cp->graph(0)->setData(m_x, m_y);
  resetView();
}

void EgTemperatureChart::rescaleAxes() {

  auto cp = (QCustomPlot *)m_cp;
  cp->graph(0)->rescaleKeyAxis();
  cp->replot();
}

void EgTemperatureChart::appendPoint(double x, double y) {
  auto cp = (QCustomPlot *)m_cp;
  if (m_x.length() > 0) {
    if (x <= m_x.last()) {
      return;
    }
  }

  m_x.append(x);
  m_y.append(y);
  cp->graph(0)->addData(x, y);
  cp->replot();
}

void EgTemperatureChart::resetView() {
  auto cp = (QCustomPlot *)m_cp;

  cp->yAxis->setRange(-20.0, 30.0);
  rescaleAxes();
}
