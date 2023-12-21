#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QMap>
#include <QVariant>

class QSettings;

class Configuration
{
  public:
    Configuration();

    enum class ConfigType
    {
        RestUrl,
        RestPort,
        MqttUrl,
        MqttPort,
        MqttUsername,
        MqttPassword
    };

    QVariant getConfiguration(const QString& type);
    void setConfiguration(ConfigType type, const QVariant& value);

    bool validated() const;
    void setValidated(bool newValidated);

  private:
    QSettings* m_settings;
    bool m_validated;
    QMap<ConfigType, QString> m_configKeys = {
        {ConfigType::RestUrl, "rest_url"},
        {ConfigType::RestPort, "rest_port"},
        {ConfigType::MqttUrl, "mqtt_url"},
        {ConfigType::MqttPort, "mqtt_port"},
        {ConfigType::MqttUsername, "mqtt_username"},
        {ConfigType::MqttPassword, "mqtt_password"},
    };

    void validate();
};

extern Configuration configuration;

#endif   // CONFIGURATION_H
