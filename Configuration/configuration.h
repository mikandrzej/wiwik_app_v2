#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QVariant>

class Configuration {
public:
  Configuration();

  QVariant getConfiguration(QString type);

private:
  QMap<QString, QVariant> m_data;
};

extern Configuration configuration;

#endif // CONFIGURATION_H
