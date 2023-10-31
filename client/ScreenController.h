#ifndef SCREENCONTROLLER_H
#define SCREENCONTROLLER_H

#include <QDialog>
#include "Client.h"

namespace Ui {
  class ScreenController;
}

class ScreenController : public QDialog{
  Q_OBJECT

  public:
    explicit ScreenController(QWidget* parent = nullptr);
    ~ScreenController();

  public slots:
    void setLoginWindow();
    void setRegisterWindow();
    void setChatWindow();
    void exit();
    void handleNetworkFailed();

  private:
    void setupSignals();
    Ui::ScreenController* ui;
    Client* client_;
};

#endif // SCREENCONTROLLER_H
