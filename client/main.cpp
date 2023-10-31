#include <QApplication>
#include <QtDebug>
#include "ScreenController.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  auto screenController = new ScreenController;
  screenController->setAttribute(Qt::WA_DeleteOnClose);
  screenController->show();
  return app.exec();
}
