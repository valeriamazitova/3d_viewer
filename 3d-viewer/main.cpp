#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setStyle("fusion");
  MainWindow w;
  w.show();
  QPalette m_pal;
  //    m_pal.setBrush(QPalette::Active, QPalette::Window, QBrush
  //    (QPixmap(":/img1/img/19.jpg")));
  w.setPalette(m_pal);
  return a.exec();
}
