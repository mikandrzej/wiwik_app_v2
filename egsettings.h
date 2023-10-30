#ifndef EGSETTINGS_H
#define EGSETTINGS_H

#include <QObject>

class EgSettings : public QObject {
  Q_OBJECT
public:
  explicit EgSettings(QObject *parent = nullptr);

  QString serverUrl() const;

signals:

  void serverUrlChanged(const QString &value);

public slots:
  void setServerUrl(const QString &newServerUrl);

private:
  QString m_serverUrl;
};

#endif // EGSETTINGS_H
