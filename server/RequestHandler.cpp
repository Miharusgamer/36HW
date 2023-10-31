#include "RequestHandler.h"

namespace{
    enum Command{
        IS_PASSWORD_CORRECT,
        IS_USER_REGISTERED,
        ADD_USER,
        ADD_COMMON_MESSAGE,
        ADD_PRIVATE_MESSAGE,
        GET_COMMON_MESSAGE,
        GET_PRIVATE_MESSAGE,
        SHOW_USERS,
        REMOVE_USER
    };
}

RequestHandler::RequestHandler(Database* database): database_(database)
{

}

QString RequestHandler::handle(int commandType, const QString& args)
{
    QString response;
    switch(commandType){
    case IS_PASSWORD_CORRECT:
        response = isPasswordCorrect(args);
        break;
    case IS_USER_REGISTERED:
        response = isUserRegistered(args);
        break;
    case ADD_USER:
        response = addUser(args);
        break;
    case ADD_COMMON_MESSAGE:
        response = addCommonMessage(args);
        break;
    case ADD_PRIVATE_MESSAGE:
        response = addPrivateMessage(args);
        break;
    case GET_COMMON_MESSAGE:
        response = getCommonMessage();
        break;
    case GET_PRIVATE_MESSAGE:
        response = getPrivateMessage(args);
        break;
    case SHOW_USERS:
        response = showUsers();
        break;
    case REMOVE_USER:
        response = removeUser(args);
        break;
    default:
        break;
    }
    return response;
}

QString RequestHandler::isPasswordCorrect(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();
    const std::string passwordHash = listArgs.at(1).toStdString();

    auto isOk = database_->isPasswordCorrect(login, passwordHash);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString RequestHandler::isUserRegistered(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();
    auto isOk = database_->isUserRegistered(login);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString RequestHandler::addUser(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();
    const std::string passwordHash = listArgs.at(1).toStdString();

    auto isOk = database_->registerUser(login, passwordHash);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString RequestHandler::addCommonMessage(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string sender = listArgs.at(0).toStdString();
    const std::string message = listArgs.at(1).toStdString();

    auto isOk = database_->addCommonMessage(sender, message);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString RequestHandler::addPrivateMessage(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string sender = listArgs.at(0).toStdString();
    const std::string receiver = listArgs.at(1).toStdString();
    const std::string message = listArgs.at(2).toStdString();

    auto isOk = database_->addPrivateMessage(sender, receiver, message);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString RequestHandler::getCommonMessage()
{
    auto messages = database_->getCommonMessages();
    auto response = QString::fromStdString(messages);
    return response;
}

QString RequestHandler::getPrivateMessage(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string receiver = listArgs.at(0).toStdString();

    auto messages = database_->getPrivateMessages(receiver);
    auto response = QString::fromStdString(messages);
    return response;
}

QString RequestHandler::showUsers()
{
    auto users = database_->getLogins();
    QString response;
    for (const auto& user : users){
        response += QString::fromStdString(user) + "|";
    }
    return response;
}

QString RequestHandler::removeUser(const QString& args)
{
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();

    auto isOk = database_->removeUser(login);
    QString response = "false";
    if (isOk){
        response = "true";
    }

    return response;
}
