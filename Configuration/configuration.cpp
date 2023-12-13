#include "configuration.h"

Configuration configuration;

Configuration::Configuration() {
  m_data["server_url"] = "http://10.22.1.57:14999";
}

QVariant Configuration::getConfiguration(QString type) {
  if (m_data.contains(type))
    return m_data[type];

  return QVariant();
}
