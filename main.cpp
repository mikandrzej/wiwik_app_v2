#include "app.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  App app(&w);
  w.show();
  return a.exec();
}
