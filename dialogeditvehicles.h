#ifndef DIALOGEDITVEHICLES_H
#define DIALOGEDITVEHICLES_H

#include <QAbstractItemModel>
#include <QDialog>

namespace Ui {
class DialogEditVehicles;
}

class DialogEditVehicles : public QDialog {
  Q_OBJECT

public:
  explicit DialogEditVehicles(QWidget *parent = nullptr);
  ~DialogEditVehicles();

  void setCarListModel(QAbstractItemModel *newCarListModel);

signals:
  void addNewVehicle(QString &vehName, QString &plateNo);

private slots:

  void on_pb_saveAsNewVehicle_clicked();

private:
  Ui::DialogEditVehicles *ui;
  QAbstractItemModel *m_carListModel;
};

#endif // DIALOGEDITVEHICLES_H
