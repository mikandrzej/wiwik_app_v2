#include "app.h"
#include "mainwindow.h"

#include <QApplication>

#include <QSslSocket>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setStyle("Fusion");
  MainWindow w;
  App app(&w);

  qDebug()<<"SSL lib:";
  qDebug()<<QSslSocket::sslLibraryBuildVersionString();
  qDebug()<<QSslSocket::sslLibraryVersionString();
  qDebug()<<QSslSocket::supportsSsl();

  w.show();
  return a.exec();
}
