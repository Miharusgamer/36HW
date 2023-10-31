#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include "Client.h"

namespace Ui {
  class RegistrationWindow;
}

class RegistrationWindow : public QDialog{
  Q_OBJECT

  public:
    explicit RegistrationWindow(QWidget* parent = nullptr);
    ~RegistrationWindow();
    void setClient(Client* client);

  public slots:
    void handleServerResponse_IsUserRegistered();
    void handleServerResponse_IsUserAdded();

  signals:
    void toLogin();
    void toChat(const std::string& userLogin);
    void close();

  private slots:
    void on_pushButton_toLogin_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

  private:
    void connectNetworkSignal_IsUserRegistered();
    void disconnectNetworkSignal_IsUserRegistered();
    void connectNetworkSignal_IsUserAdded();
    void disconnectNetworkSignal_IsUserAdded();
    void checkIsUserRegistered(const QString& login);

    Ui::RegistrationWindow* ui;
    Client* client_;
    QString login_;
    QString password_;
};

#endif // REGISTRATIONWINDOW_H
