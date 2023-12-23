#include "egsettings.h"
#include <QDebug>

EgSettings::EgSettings(QObject* parent) : QObject {parent} {}

void EgSettings::setServerUrl(const QString& newServerUrl)
{
    if (m_serverUrl == newServerUrl)
        return;

    qDebug() << "new server URL: " << newServerUrl;
    m_serverUrl = newServerUrl;
    emit serverUrlChanged(newServerUrl);
}
