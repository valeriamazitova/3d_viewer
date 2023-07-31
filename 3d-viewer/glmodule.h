#ifndef GLMODULE_H
#define GLMODULE_H

#define GL_SILENCE_DEPRECATION

#include <QImage>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include "qgifimage.h"

extern "C" {
#include "parser.h"
}

class GLModule : public QOpenGLWidget {
  Q_OBJECT
 private:
  void normalize(viewer3D *key);
  double findMin(char plane, viewer3D *key);
  double findMax(char plane, viewer3D *key);
  double max_of_3values(double a, double b, double c);
  //    void scale(double value, double dmax, viewer3D *key);
  int indexTime = 0;
  QGifImage *gifImage = NULL;

 public:
  GLModule(QWidget *parent);
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void freeStructAndStart();
  void viewer();
  void loadSettings();
  void saveSettings();
  void record();
  int checkTimer = 1;

  viewer3D obj = {1, 0, (double *)malloc(sizeof(double)),
                  1, 0, (int *)malloc(sizeof(int)),
                  0};
  QString filepath;
  FILE *file = NULL;
  int typeProjection = 0;  // 0 - Ортографическая, 1 - Перспективная проекции
  double redBack = 0.0, greenBack = 0.0, blueBack = 0.0;
  double redLine = 1.0, greenLine = 0.0, blueLine = 1.0;
  double redPoint = 0.0, greenPoint = 1.0, bluePoint = 1.0;
  double xRot = 0.0, yRot = 0.0, zRot = 0.0;
  QPoint mPos;
  double xPos = 0.0, yPos = 0.0, zPos = 0.0;
  double start_x = 0.0, start_y = 0.0;
  double prev_scale = 1;

  double lineDepth = 1.0, dotDepth = 1.0;
  int dottedLine = 0;  // 0 - сплошная, 1 - пунктир
  int dotType = 1;  // 0 - отсутствует отображение точек, 1 - круг, 2 - квадрат
  int save = 0;

  QSettings settings;

  QTimer *timer = NULL;
  QString nameGif;

 private slots:
  void gif();

  //     void jpegFile(QString name);
 signals:
  void stopRecord();
};

#endif  // GLMODULE_H
