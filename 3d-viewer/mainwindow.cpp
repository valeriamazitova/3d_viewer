#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setFixedSize(this->size());
  if (this->ui->glwidget->settings.value("save").toInt() == 1) {
    ui->dot_spinBox->setValue(
        this->ui->glwidget->settings.value("dotDepth").toInt());
    ui->line_spinBox->setValue(
        this->ui->glwidget->settings.value("lineDepth").toDouble());
    ui->sqr_point_checkBox->setCurrentIndex(
        this->ui->glwidget->settings.value("dotType").toInt());
    ui->checkLines->setChecked(
        this->ui->glwidget->settings.value("dottedLine").toInt());
    ui->frustum_check->setChecked(
        this->ui->glwidget->settings.value("projection").toInt());
  }
  connect(ui->glwidget, SIGNAL(stopRecord()), this, SLOT(changeColor()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_open_file_Button_clicked() {
  QString filters("OBJ (*.obj)");
  QString file_name = QFileDialog::getOpenFileName(this, "Open file",
                                                   "../../../../obj", filters);
  ;
  if (file_name.isEmpty() == false) {
    this->ui->glwidget->filepath = file_name;
    this->ui->glwidget->freeStructAndStart();
    this->ui->glwidget->update();
  }
  ui->labelFileName->setText(file_name.section("/", -1, -1));
  ui->labelCountOfVertex->setText(
      QString::number(this->ui->glwidget->obj.lastIndexPoint / 3));
  ui->labelCountOfFacets->setText(
      QString::number(this->ui->glwidget->obj.countFacete));
}

// void MainWindow::on_jpeg_pushButton_clicked() {
//   QString filters("JPEG (*.jpeg);;BMP (*.bmp)");
//   QString defaultFilter("JPEG (*.jpeg)");
//   QString fname_screen = QFileDialog::getSaveFileName(
//       0, "Save file", QDir::currentPath(), filters, &defaultFilter);
////   this->ui->glwidget->jpegFile(fname_screen);
// }

// void MainWindow::on_pbSaveDirectory_clicked() {
//   QString filters("GIF (*.gif)");
//   QString defaultFilter("GIF (*.gif)");
////   this->ui->GLwidget->fname_gif = "";
////   this->ui->GLwidget->fname_gif = QFileDialog::getSaveFileName(
////       0, "Save GIF", QDir::currentPath(), filters, &defaultFilter);
////   ui->leDirectory->setText(this->ui->GLwidget->fname_gif);
//////   check_open_file_button = 1;
// }

void MainWindow::on_color_Button_clicked() {
  QColor colors;
  colors = QColorDialog::getColor(Qt::white, this);
  if (ui->comboBox->currentIndex() == 0) {
    this->ui->glwidget->redLine = colors.redF();
    this->ui->glwidget->greenLine = colors.greenF();
    this->ui->glwidget->blueLine = colors.blueF();

  } else if (ui->comboBox->currentIndex() == 1) {
    this->ui->glwidget->redPoint = colors.redF();
    this->ui->glwidget->greenPoint = colors.greenF();
    this->ui->glwidget->bluePoint = colors.blueF();

  } else if (ui->comboBox->currentIndex() == 2) {
    this->ui->glwidget->redBack = colors.redF();
    this->ui->glwidget->greenBack = colors.greenF();
    this->ui->glwidget->blueBack = colors.blueF();
  }
  this->ui->glwidget->update();
}

void MainWindow::on_line_spinBox_valueChanged(double arg1) {
  this->ui->glwidget->lineDepth = arg1;
  this->ui->glwidget->update();
}

void MainWindow::on_dot_spinBox_valueChanged(int arg1) {
  this->ui->glwidget->dotDepth = arg1;
  this->ui->glwidget->update();
}

void MainWindow::on_sqr_point_checkBox_currentIndexChanged() {
  if (ui->sqr_point_checkBox->currentIndex() == 0) {
    this->ui->glwidget->dotType = 1;
  } else if (ui->sqr_point_checkBox->currentIndex() == 1) {
    this->ui->glwidget->dotType = 0;
  } else if (ui->sqr_point_checkBox->currentIndex() == 2) {
    this->ui->glwidget->dotType = 2;
  }
  this->ui->glwidget->update();
}

void MainWindow::on_checkLines_clicked(bool checked) {
  this->ui->glwidget->dottedLine = checked;
  this->ui->glwidget->update();
}

void MainWindow::on_frustum_check_clicked(bool checked) {
  this->ui->glwidget->typeProjection = checked;
  this->ui->glwidget->update();
}

void MainWindow::on_pushButton_moveXYZ_clicked() {
  double step_x = ui->transl_x_SpinBox->value();
  double step_y = ui->trans_y_SpinBox->value();
  double step_z = ui->trans_z_SpinBox->value();

  for (int i = 0; i < this->ui->glwidget->obj.lastIndexPoint; i += 3) {
    this->ui->glwidget->obj.arrayPoint[i] += step_x;
    this->ui->glwidget->obj.arrayPoint[i + 1] += step_y;
    this->ui->glwidget->obj.arrayPoint[i + 2] += step_z;
  }
  this->ui->glwidget->update();
}

void MainWindow::on_pushButton_rotateXYZ_clicked() {
  double rot_x = ui->rotate_x_SpinBox->value();
  double rot_y = ui->rotate_y_SpinBox->value();
  double rot_z = ui->rotate_z_SpinBox->value();

  rotateViewer(&(this->ui->glwidget->obj), rot_x, 'x');
  rotateViewer(&(this->ui->glwidget->obj), rot_y, 'y');
  rotateViewer(&(this->ui->glwidget->obj), rot_z, 'z');
  this->ui->glwidget->update();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1) {
  scaleViewer(&(this->ui->glwidget->obj), 1 / (this->ui->glwidget->prev_scale));
  scaleViewer(&(this->ui->glwidget->obj), arg1);
  this->ui->glwidget->prev_scale = arg1;
  this->ui->glwidget->update();
}

void MainWindow::on_transl_x_SpinBox_valueChanged(double arg1) {
  for (int i = 0; i < this->ui->glwidget->obj.lastIndexPoint; i += 3) {
    this->ui->glwidget->obj.arrayPoint[i] -= this->ui->glwidget->xPos;
    this->ui->glwidget->obj.arrayPoint[i] += arg1;
  }
  this->ui->glwidget->xPos = arg1;
  this->ui->glwidget->update();
}

void MainWindow::on_trans_y_SpinBox_valueChanged(double arg1) {
  for (int i = 1; i < this->ui->glwidget->obj.lastIndexPoint; i += 3) {
    this->ui->glwidget->obj.arrayPoint[i] -= this->ui->glwidget->yPos;
    this->ui->glwidget->obj.arrayPoint[i] += arg1;
  }
  this->ui->glwidget->yPos = arg1;
  this->ui->glwidget->update();
}

void MainWindow::on_trans_z_SpinBox_valueChanged(double arg1) {
  for (int i = 2; i < this->ui->glwidget->obj.lastIndexPoint; i += 3) {
    this->ui->glwidget->obj.arrayPoint[i] -= this->ui->glwidget->zPos;
    this->ui->glwidget->obj.arrayPoint[i] += arg1;
  }
  this->ui->glwidget->zPos = arg1;
  this->ui->glwidget->update();
}

void MainWindow::on_rotate_x_SpinBox_valueChanged(double arg1) {
  rotateViewer(&(this->ui->glwidget->obj), arg1, 'x');
  this->ui->glwidget->update();
}

void MainWindow::on_rotate_y_SpinBox_valueChanged(double arg1) {
  rotateViewer(&(this->ui->glwidget->obj), arg1, 'y');
  this->ui->glwidget->update();
}

void MainWindow::on_rotate_z_SpinBox_valueChanged(double arg1) {
  rotateViewer(&(this->ui->glwidget->obj), arg1, 'z');
  this->ui->glwidget->update();
}

void MainWindow::on_save_jpeg_clicked() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", "~/ourViewer/saved/name.jpeg", tr("JPEG (*.jpeg)"));
  ui->glwidget->grabFramebuffer().save(file, NULL, 100);
}

void MainWindow::on_save_bmp_clicked() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", "../saved/name.bmp", tr("BMP (*.bmp)"));
  ui->glwidget->grabFramebuffer().save(file, NULL, 100);
}

void MainWindow::on_save_gif_clicked() {
  QString name = QFileDialog::getSaveFileName(
      this, "Save as...", "../saved/name.gif", tr("GIF (*.gif)"));
  if (!name.isEmpty() && this->ui->glwidget->checkTimer == 1) {
    ui->save_gif->setStyleSheet("background-color:#FF0000");
    this->ui->glwidget->nameGif = name;
    this->ui->glwidget->record();
  }
}

void MainWindow::changeColor() {
  this->ui->save_gif->setStyleSheet(
      "QPushButton {background-color: rgb(102, 192, 220);color:white;} "
      "QPushButton:hover{background-color: rgb(152, 185, 255);color:rgb(78, "
      "120, 254);} ");
}
