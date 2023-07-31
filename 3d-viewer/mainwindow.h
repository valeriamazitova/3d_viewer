#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_open_file_Button_clicked();
  void on_color_Button_clicked();
  //    void on_jpeg_pushButton_clicked();
  //    void on_pbSaveDirectory_clicked();
  void on_line_spinBox_valueChanged(double arg1);
  void on_dot_spinBox_valueChanged(int arg1);
  void on_sqr_point_checkBox_currentIndexChanged();
  void on_checkLines_clicked(bool checked);
  void on_frustum_check_clicked(bool checked);
  void on_pushButton_moveXYZ_clicked();
  void on_pushButton_rotateXYZ_clicked();
  void on_doubleSpinBox_valueChanged(double arg1);
  void on_transl_x_SpinBox_valueChanged(double arg1);
  void on_trans_y_SpinBox_valueChanged(double arg1);
  void on_trans_z_SpinBox_valueChanged(double arg1);
  void on_rotate_x_SpinBox_valueChanged(double arg1);
  void on_rotate_y_SpinBox_valueChanged(double arg1);
  void on_rotate_z_SpinBox_valueChanged(double arg1);
  void on_save_jpeg_clicked();
  void on_save_bmp_clicked();
  void on_save_gif_clicked();
  void changeColor();

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
