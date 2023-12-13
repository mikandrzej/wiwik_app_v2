#ifndef MEASURE_H
#define MEASURE_H

#include <QDateTime>
#include <QObject>
#include <QVariant>
class Measure : public QObject {
  Q_OBJECT
public:
  explicit Measure(QDateTime &timestamp, QVariant value,
                   QObject *parent = nullptr);
  explicit Measure(Measure *measure, QObject *parent = nullptr);

  QVariant value() const;

  QDateTime timestamp() const;

signals:

private:
  QVariant m_value;
  QDateTime m_timestamp;
};

#endif // MEASURE_H
