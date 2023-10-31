#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "Client.h"

namespace Ui {
  class LoginWindow;
}

class LoginWindow : public QDialog{
  Q_OBJECT

  public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();
    void setClient(Client* client);

  signals:
    void toRegistration();
    void toChat(const std::string& userLogin);
    void close();

  public slots:
    void handleServerResponse_IsPasswordCorrect();

  private slots:
    void on_pushButton_Registration_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

  private:
    void connectNetworkSignal_IsPasswordCorrect();
    void disconnectNetworkSignal_IsPasswordCorrect();
    Ui::LoginWindow* ui;
    Client* client_;
};

#endif // LOGINWINDOW_H
