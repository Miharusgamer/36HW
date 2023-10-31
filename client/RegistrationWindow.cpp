#include "RegistrationWindow.h"
#include "ui_RegistrationWindow.h"
#include <QMessageBox>
#include "SHA1Wrapper.h"
#include "InputChecker.h"

RegistrationWindow::RegistrationWindow(QWidget* parent) : QDialog(parent), ui(new Ui::RegistrationWindow)
{
  ui->setupUi(this);
}

RegistrationWindow::~RegistrationWindow()
{
  delete ui;
}

void RegistrationWindow::setClient(Client* client)
{
  client_ = client;
}

void RegistrationWindow::handleServerResponse_IsUserRegistered()
{
  auto isUserRegistered = client_->getResponse();
  disconnectNetworkSignal_IsUserRegistered();

  if (isUserRegistered == "true"){
    QString message = "User already exists. Try to log in.";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  connectNetworkSignal_IsUserAdded();

  auto hashPassword = sha_1::hash(password_.toStdString());
  connectNetworkSignal_IsUserAdded();
  client_->requestAddUser(login_, QString::fromStdString(hashPassword));
}

void RegistrationWindow::handleServerResponse_IsUserAdded()
{
  auto isUserAdded = client_->getResponse();
  disconnectNetworkSignal_IsUserAdded();

  if (isUserAdded == "true"){
    auto login = ui->lineEdit_Login->text().toStdString();
    ui->lineEdit_Login->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ConfirmPassword->clear();

    emit toChat(login);
  }
  else{
    QString message = "User is not added - something wrong";
    QMessageBox::critical(this, "Error", message);
    return;
  }
}

void RegistrationWindow::on_pushButton_toLogin_clicked()
{
  ui->lineEdit_Login->clear();
  ui->lineEdit_Password->clear();
  ui->lineEdit_ConfirmPassword->clear();

  emit toLogin();
}

void RegistrationWindow::on_buttonBox_accepted()
{
  login_.clear();
  password_.clear();

  auto login = ui->lineEdit_Login->text().toStdString();
  if (login.empty()){
    QString message = "Please type Login";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isContainsForbidenChars(login)){
    QString message = "Login contains forbidden symbols. Allowed: 'a'-'z', 'A'-'Z', '0'-'9')";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isStartsByNumber(login)){
    QString message = "Login starts by number";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isTooShort(login)){
    QString message = "Login too short. Min length: " + QString::number(InputChecker::getMinLength()) + " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  auto password = ui->lineEdit_Password->text().toStdString();
  if (password.empty()){
    QString message = "Please type Password";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isTooShort(password)){
    QString message = "Password too short. Min length: " + QString::number(InputChecker::getMinLength()) + " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  auto confirmPassword = ui->lineEdit_ConfirmPassword->text().toStdString();
  if (confirmPassword.empty()){
    QString message = "Please type Confirm Password";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (password != confirmPassword){
    QString message = "Password doesn't match Confirm Password";
    QMessageBox::critical(this, "Error", "Password doesn't match Confirm Password");
    return;
  }

  login_ = QString::fromStdString(login);
  password_ = QString::fromStdString(password);

  checkIsUserRegistered(login_);
}

void RegistrationWindow::on_buttonBox_rejected()
{
  emit close();
}

void RegistrationWindow::connectNetworkSignal_IsUserRegistered()
{
  connect(client_, &Client::dataReceived, this, &RegistrationWindow::handleServerResponse_IsUserRegistered);
}

void RegistrationWindow::disconnectNetworkSignal_IsUserRegistered()
{
  disconnect(client_, &Client::dataReceived, this, &RegistrationWindow::handleServerResponse_IsUserRegistered);
}

void RegistrationWindow::connectNetworkSignal_IsUserAdded()
{
  connect(client_, &Client::dataReceived, this, &RegistrationWindow::handleServerResponse_IsUserAdded);
}

void RegistrationWindow::disconnectNetworkSignal_IsUserAdded()
{
  disconnect(client_, &Client::dataReceived, this, &RegistrationWindow::handleServerResponse_IsUserAdded);
}

void RegistrationWindow::checkIsUserRegistered(const QString& login)
{
  connectNetworkSignal_IsUserRegistered();
  client_->requestIsUserRegistered(login);
}
