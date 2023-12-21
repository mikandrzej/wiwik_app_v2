#include "configuration.h"
#include <QDebug>
#include <QSettings>
#include <QtWidgets/QDialog>

Configuration configuration;

Configuration::Configuration()
{
    m_settings = new QSettings("Toucan IT", "WiwikApp");

    m_settings->setValue("server_url", "http://iot.2canit.pl:14999");
    validate();

    // qDebug() << "Settings: " << settings.allKeys();
    // qDebug() << "Settings file: " << settings.fileName();

    // QDialog dialog;
    // dialog.show();
}

QVariant Configuration::getConfiguration(const QString& type)
{
    if (m_settings->contains(type))
        return m_settings->value(type);

    return {};
}

void Configuration::setConfiguration(ConfigType /*type*/, const QVariant& /*value*/) {}

bool Configuration::validated() const
{
    return m_validated;
}

void Configuration::setValidated(bool newValidated)
{
    m_validated = newValidated;
}

void Configuration::validate()
{
    for (auto& key : m_configKeys)
    {
        if (!m_settings->contains(key))
        {
            m_validated = false;
            return;
        }
    }

    m_validated = true;
}
