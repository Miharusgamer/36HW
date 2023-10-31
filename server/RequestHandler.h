#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QString>
#include "Database.h"

class RequestHandler{
  public:
    RequestHandler() = delete;
    explicit RequestHandler(Database* database);
    QString handle(int commandType, const QString& args);

  private:
    QString isPasswordCorrect(const QString& args);
    QString isUserRegistered(const QString& args);
    QString addUser(const QString& args);
    QString addCommonMessage(const QString& args);
    QString addPrivateMessage(const QString& args);
    QString getCommonMessage();
    QString getPrivateMessage(const QString& args);
    QString showUsers();
    QString removeUser(const QString& args);

    Database* database_;
};

#endif // REQUESTHANDLER_H
