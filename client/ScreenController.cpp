#include "ScreenController.h"
#include "ui_ScreenController.h"
#include <QtDebug>
#include <QMessageBox>
#include "SHA1Wrapper.h"

namespace{
  enum{
    LOGIN_WINDOW = 0,
    REGISTRATION_WINDOW,
    CHAT_WINDOW
  };
}

ScreenController::ScreenController(QWidget* parent):
  QDialog(parent), ui(new Ui::ScreenController)
{
  ui->setupUi(this);

  client_ = new Client("127.0.0.1", 7777);

  ui->loginWindow->setClient(client_);
  ui->registrationWindow->setClient(client_);
  ui->chatWindow->setClient(client_);

  setLoginWindow();
  setupSignals();
}

ScreenController::~ScreenController()
{
  delete ui;
  qApp->exit(0);
}

void ScreenController::setLoginWindow()
{
  ui->stackedWidget->setCurrentIndex(LOGIN_WINDOW);
}

void ScreenController::setRegisterWindow()
{
  ui->stackedWidget->setCurrentIndex(REGISTRATION_WINDOW);
}

void ScreenController::setChatWindow()
{
  ui->stackedWidget->setCurrentIndex(CHAT_WINDOW);
  ui->chatWindow->connectNetworkSignal_UpdateMessage();
}

void ScreenController::exit()
{
  this->close();
}

void ScreenController::handleNetworkFailed()
{
  QString message = "Server connection failed. Contact the server administrator or try to connect later.";
  QMessageBox::critical(this, "Error", message);
  this->close();
}

void ScreenController::setupSignals()
{ 
  connect(client_, &Client::failConnection,
        this, &ScreenController::handleNetworkFailed);

  connect(ui->loginWindow, &LoginWindow::close,
        this, &ScreenController::exit);

  connect(ui->registrationWindow, &RegistrationWindow::close,
        this, &ScreenController::exit);

  connect(ui->loginWindow, &LoginWindow::toRegistration,
          this, &ScreenController::setRegisterWindow);

  connect(ui->loginWindow, &LoginWindow::toChat,
          this, &ScreenController::setChatWindow);

  connect(ui->loginWindow, &LoginWindow::toChat,
          ui->chatWindow, &ChatWindow::setUserLogin);

  connect(ui->registrationWindow, &RegistrationWindow::toLogin,
          this, &ScreenController::setLoginWindow);
  connect(ui->registrationWindow, &RegistrationWindow::toChat,
          this, &ScreenController::setChatWindow);

  connect(ui->registrationWindow, &RegistrationWindow::toChat,
          ui->chatWindow, &ChatWindow::setUserLogin);

  connect(ui->chatWindow, &ChatWindow::toLogin,
          this, &ScreenController::setLoginWindow);
}
