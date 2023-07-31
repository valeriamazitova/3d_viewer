#include "glmodule.h"

GLModule::GLModule(QWidget *parent) : QOpenGLWidget(parent) {}

void GLModule::normalize(viewer3D *key) {
  double min_x = findMin('x', key);
  double min_y = findMin('y', key);
  double min_z = findMin('z', key);
  double max_x = findMax('x', key);
  double max_y = findMax('y', key);
  double max_z = findMax('z', key);

  double mean_x = max_x - min_x;
  double mean_y = max_y - min_y;
  double mean_z = max_z - min_z;

  double center_x = min_x + (max_x - min_x) / 2;
  double center_y = min_y + (max_y - min_y) / 2;
  double center_z = min_z + (max_z - min_z) / 2;

  for (int i = 0; i < key->lastIndexPoint;
       i += 3) {  // отцентровывает изображение
    key->arrayPoint[i] -= center_x;
    key->arrayPoint[i + 1] -= center_y;
    key->arrayPoint[i + 2] -= center_z;
  }

  double dmax = max_of_3values(mean_x, mean_y, mean_z);

  double value = 1;
  double scale =
      (value - (value * (-3))) / dmax;  // подгоняет его под рамки поля
  scaleViewer(key, scale);
}

double GLModule::findMin(char plane, viewer3D *key) {
  int start = 0;
  if (plane == 'x') {
    start = 0;
  } else if (plane == 'y') {
    start = 1;
  } else if (plane == 'z') {
    start = 2;
  }
  double findMin = key->arrayPoint[start];
  for (int i = start; i < key->lastIndexPoint; i += 3) {
    if (key->arrayPoint[i] < findMin) {
      findMin = key->arrayPoint[i];
    }
  }
  return findMin;
}

double GLModule::findMax(char plane, viewer3D *key) {
  int start = 0;
  if (plane == 'x') {
    start = 0;
  } else if (plane == 'y') {
    start = 1;
  } else if (plane == 'z') {
    start = 2;
  }
  double findMax = key->arrayPoint[start];
  for (int i = start; i < key->lastIndexPoint; i += 3) {
    if (key->arrayPoint[i] > findMax) {
      findMax = key->arrayPoint[i];
    }
  }
  return findMax;
}

double GLModule::max_of_3values(double a, double b, double c) {
  double max = 0.0;
  if (a >= b && a >= c)
    max = a;
  else if (b >= a && b >= c)
    max = b;
  else
    max = c;
  return max;
}

void GLModule::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  loadSettings();
}

void GLModule::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLModule::paintGL() {
  float size = 3, fov = 60.0 * M_PI / 180,
        far = 15;  // far можно задать любым, лишь бы все умещалось.
  float heapHeight = size / (2 * tan(fov / 2));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (typeProjection == 0) {
    glOrtho(-3, 3, -3, 3, -15, 15);
  } else {
    glFrustum(-size, size, -size, size, heapHeight, far);
    //    glFrustum(-1.5, 1.5, -1.5, 1.5, 1, 5);
  }
  glClearColor(redBack, greenBack, blueBack, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -3 * heapHeight);
  saveSettings();
  viewer();
}

void GLModule::mousePressEvent(QMouseEvent *mo) {
  mPos = mo->pos();
  start_x = mo->pos().x();
  start_y = mo->pos().y();
}

void GLModule::mouseMoveEvent(QMouseEvent *mo) {
  xRot = 1 / M_PI * (mo->pos().y() - start_y);
  yRot = 1 / M_PI * (mo->pos().x() - start_x);
  rotateViewer(&obj, xRot, 'x');
  rotateViewer(&obj, yRot, 'y');
  start_x = mo->pos().x();
  start_y = mo->pos().y();
  update();
}

void GLModule::freeStructAndStart() {
  obj.sizeArrayFacete = 1;
  obj.sizeArrayPoint = 1;
  obj.countFacete = 0;
  obj.lastIndexFacete = 0;
  obj.lastIndexPoint = 0;
  std::string s = filepath.toStdString();
  char *s1 = &s[0];
  if (!openFile(s1, &file)) {
    parse(file, &obj);
    normalize(&obj);
    fclose(file);
  }
  xRot = yRot = zRot = 0.0;
  xPos = yPos = zPos = 0.0;
  prev_scale = 1;
}

void GLModule::viewer() {
  glVertexPointer(3, GL_DOUBLE, 0, obj.arrayPoint);
  glEnableClientState(GL_VERTEX_ARRAY);
  if (dotType) {
    if (dotType == 1) glEnable(GL_POINT_SMOOTH);
    glColor3d(redPoint, greenPoint, bluePoint);
    glPointSize(dotDepth);
    glDrawArrays(GL_POINTS, 0, obj.lastIndexPoint / 3);
  }
  glEnable(GL_LINE_STIPPLE);
  glLineWidth(lineDepth);
  glColor3d(redLine, greenLine, blueLine);
  if (dottedLine)
    glLineStipple(2, 0x00FF);
  else
    glLineStipple(2, 0xFFFF);
  glDrawElements(GL_LINES, obj.lastIndexFacete, GL_UNSIGNED_INT,
                 obj.arrayFacete);
  if (dotType == 1) glDisable(GL_POINT_SMOOTH);
  glDisable(GL_LINE_STIPPLE);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GLModule::loadSettings() {
  save = settings.value("save").toInt();
  if (save == 1) {
    redLine = settings.value("redLine").toDouble();
    greenLine = settings.value("greenLine").toDouble();
    blueLine = settings.value("blueLine").toDouble();
    redPoint = settings.value("redPoint").toDouble();
    greenPoint = settings.value("greenPoint").toDouble();
    bluePoint = settings.value("bluePoint").toDouble();
    redBack = settings.value("redBack").toDouble();
    greenBack = settings.value("greenBack").toDouble();
    blueBack = settings.value("blueBack").toDouble();
    dotDepth = settings.value("dotDepth").toDouble();
    lineDepth = settings.value("lineDepth").toDouble();
    dottedLine = settings.value("dottedLine").toInt();
    dotType = settings.value("dotType").toInt();
  }
}

void GLModule::saveSettings() {
  settings.setValue("redLine", redLine);
  settings.setValue("greenLine", greenLine);
  settings.setValue("blueLine", blueLine);
  settings.setValue("redPoint", redPoint);
  settings.setValue("greenPoint", greenPoint);
  settings.setValue("bluePoint", bluePoint);
  settings.setValue("redBack", redBack);
  settings.setValue("greenBack", greenBack);
  settings.setValue("blueBack", blueBack);
  settings.setValue("save", 1);
  settings.setValue("dotDepth", dotDepth);
  settings.setValue("lineDepth", lineDepth);
  settings.setValue("dottedLine", dottedLine);
  settings.setValue("dotType", dotType);
}

void GLModule::record() {
  gifImage = new QGifImage();
  timer = new QTimer();
  checkTimer = 0;
  timer->start(100);
  connect(timer, SIGNAL(timeout()), this, SLOT(gif()));
}

void GLModule::gif() {
  indexTime++;
  gifImage->addFrame(GLModule::grabFramebuffer());
  if (indexTime == 50) {
    timer->stop();
    gifImage->save(nameGif);
    indexTime = 0;
    emit stopRecord();
    checkTimer = 1;
    delete timer;
    delete gifImage;
  }
}
