#include <QApplication>
#include <QDesktopWidget>
#include <QWindow>
#include "drawing_lib/com_interface.h"

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  QDesktopWidget screen;
  ComInterface com_interface;

  return app.exec();
}
