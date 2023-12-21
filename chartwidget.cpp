#include "chartwidget.h"
#include <QList>
#include "QCustomPlot/qcustomplot.h"

static QColor getRandomColor()
{
    constexpr QColor color_map[] = {
        QColorConstants::Black,
        QColorConstants::DarkGray,
        QColorConstants::LightGray,
        QColorConstants::Red,
        QColorConstants::Green,
        QColorConstants::Blue,
        QColorConstants::Cyan,
        QColorConstants::Magenta,
        QColorConstants::DarkRed,
        QColorConstants::DarkGreen,
        QColorConstants::DarkBlue,
        QColorConstants::DarkCyan,
        QColorConstants::DarkMagenta,
        QColorConstants::DarkYellow,
    };
    constexpr int color_cnt = sizeof(color_map) / sizeof(color_map[0]);

    static int current_idx = 0;

    const auto color = color_map[current_idx];

    current_idx++;
    current_idx %= color_cnt;

    return color;
}

ChartWidget::ChartWidget(QWidget* parent)
{
    m_cp = new QCustomPlot(parent);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_cp);

    m_cp->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    m_cp->legend->setVisible(true);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm");
    dateTicker->setTimeZone(QTimeZone::systemTimeZone());
    m_cp->xAxis->setLabel("Godzina");
    m_cp->xAxis->setTicker(dateTicker);
    dateTicker->setTickCount(24);
    m_cp->yAxis->setLabel("Temperatura");
}

GraphData* ChartWidget::addGraph(QString& reference, const QString& name)
{
    auto graphData = new GraphData(this, reference);
    m_graphsData.append(graphData);

    auto graph = m_cp->addGraph();
    graphData->graph = graph;

    auto color = getRandomColor();
    graph->setPen(QPen(color));
    color.setAlpha(20);
    graph->setBrush(QBrush(color));

    graph->setName(name);

    return graphData;
}

void ChartWidget::addData(double x, double y, const QString& chart_ref)
{
    for (auto& graphData : m_graphsData)
    {
        if (graphData->reference() == chart_ref)
        {
            graphData->appendData(x, y);
            break;
        }
    }
}

void ChartWidget::setAxisRange(Qt::Axis axis, const QVariant& min, const QVariant& max)
{
    bool ok;
    double min_value = {min.toDouble(&ok)};
    if (!ok)
        throw std::invalid_argument("Unable to convert min argument");
    double max_value = {max.toDouble(&ok)};
    if (!ok)
        throw std::invalid_argument("Unable to convert max argument");
    switch (axis)
    {
        case Qt::XAxis:
            m_cp->xAxis->setRange(min_value, max_value);
            break;
        case Qt::YAxis:
            m_cp->yAxis->setRange(min_value - 2, max_value + 2);
            break;
        default:
            throw std::invalid_argument("Invalid axis argument");
            break;
    }
}

void ChartWidget::setAxisTitle(Qt::Axis axis, const QString& title)
{
    switch (axis)
    {
        case Qt::XAxis:
            m_cp->xAxis->setLabel(title);
            break;
        case Qt::YAxis:
            m_cp->yAxis->setLabel(title);
            break;
        default:
            throw std::invalid_argument("Invalid axis argument");
            break;
    }
}

void ChartWidget::replot()
{
    m_cp->replot();
}

void ChartWidget::rescaleAxis(Qt::Axis axis)
{
    QCPRange range;
    bool first = true;
    bool found;

    switch (axis)
    {
        case Qt::XAxis:
            for (auto graph_data : qAsConst(m_graphsData))
            {
                if (first)
                {
                    range = graph_data->graph->getKeyRange(found, QCP::sdBoth);
                    first = false;
                }
                else
                {
                    range.expand(graph_data->graph->getKeyRange(found, QCP::sdBoth));
                }
            }
            m_cp->xAxis->setRange(range);
            m_cp->replot();
            break;
        case Qt::YAxis:
            for (auto graph_data : qAsConst(m_graphsData))
            {
                if (first)
                {
                    range = graph_data->graph->getValueRange(found, QCP::sdBoth);
                    first = false;
                }
                else
                {
                    range.expand(graph_data->graph->getValueRange(found, QCP::sdBoth));
                }
            }
            range.lower -= 2;
            range.upper += 2;
            m_cp->yAxis->setRange(range);
            m_cp->replot();
            break;
        default:
            throw std::invalid_argument("Invalid axis argument");
            break;
    }
}

void ChartWidget::clearGraphs()
{
    for (auto graph_data : qAsConst(m_graphsData))
    {
        m_cp->removeGraph(graph_data->graph);
    }
    m_graphsData.clear();
}

GraphData::GraphData(ChartWidget* parent, QString& chartRef) : parent(parent), m_reference(chartRef) {}

void GraphData::appendData(double x, double y, bool replot)
{
    graph->addData(x, y);

    if (replot)
        parent->replot();
}

void GraphData::setData(QVector<double>& x, QVector<double>& y, bool replot)
{
    graph->setData(x, y, true);

    if (replot)
        parent->replot();
}

QString GraphData::reference() const
{
    return m_reference;
}

void GraphData::clearData()
{
    graph->setData({}, {}, true);
}
