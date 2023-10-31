#include "Database.h"
#include <QtDebug>

namespace {
const std::string ALL = "ALL";
}

Database::Database() : database_(QSqlDatabase::addDatabase("QSQLITE"))
{
    database_.setDatabaseName("./chatDB.db");
    if (database_.open()) {
        qDebug() << "Database is opened";
    }
    else {
        qDebug() << "Database is not opened";
    }

    query_ = new QSqlQuery(database_);
    query_->exec("CREATE TABLE IF NOT EXISTS Users(Login TEXT, PasswordHash TEXT, Ban BOOL);");
    query_->exec("CREATE TABLE IF NOT EXISTS Messages(Sender TEXT, Receiver TEXT, Message TEXT);");
}

bool Database::isPasswordCorrect(const std::string& login, const std::string& passwordHash) const
{
    if (login.empty() || passwordHash.empty()) {
        return false;
    }
    if (!isUserRegistered(login)) {
        return false;
    }

    auto request = "SELECT PasswordHash FROM Users WHERE Login = '" + QString::fromStdString(login) + "';";
    query_->exec(request);

    QString databasePasswordHash;
    if (query_->next()) {
        databasePasswordHash = query_->value(0).toString();
    }

    return (databasePasswordHash == QString::fromStdString(passwordHash));
}

bool Database::isUserRegistered(const std::string& login) const
{
    if (login.empty()) {
        return false;
    }

    auto request = "SELECT * FROM Users WHERE Login = '" + QString::fromStdString(login) + "';";
    query_->exec(request);
    return query_->next();
}

bool Database::registerUser(const std::string& login, const std::string& passwordHash)
{
    if (login.empty() || passwordHash.empty()) {
        return false;
    }

    if (isUserRegistered(login)) {
        return false;
    }

    auto request = "INSERT INTO Users(Login, PasswordHash, Ban) VALUES('" + QString::fromStdString(login) + "', '" + QString::fromStdString(passwordHash) + "', false);";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

bool Database::addCommonMessage(const std::string& sender, const std::string& message)
{
    if (message.empty()) {
        return false;
    }

    if (!isUserRegistered(sender)) {
        return false;
    }

    if (getIsBanned(sender)) {
        return false;
    }

    auto request = "INSERT INTO Messages(Sender, Receiver, Message) VALUES('" + QString::fromStdString(sender) + "', '" + QString::fromStdString(ALL) + "', '" + QString::fromStdString(message) + "');";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

bool Database::addPrivateMessage(const std::string& sender, const std::string& receiver, const std::string& message)
{
    if (message.empty()) {
        return false;
    }

    if (!isUserRegistered(sender) || !isUserRegistered(receiver)) {
        return false;
    }

    if (getIsBanned(sender)) {
        return false;
    }

    auto request = "INSERT INTO Messages(Sender, Receiver, Message) VALUES('" + QString::fromStdString(sender) + "', '" + QString::fromStdString(receiver) + "', '" + QString::fromStdString(message) + "');";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

std::string Database::getCommonMessages() const
{
    auto request = "SELECT * FROM Messages WHERE Receiver = '" + QString::fromStdString(ALL) + "';";
    query_->exec(request);

    std::string messages;
    while (query_->next()) {
        auto sender = query_->value(0).toString();
        auto text = query_->value(2).toString();

        auto line = "<" + sender + ">: " + text + "\n";
        messages.append(line.toStdString());
    }
    return messages;
}

std::string Database::getPrivateMessages(const std::string& receiver) const
{
    auto request = "SELECT * FROM Messages WHERE (Receiver = '" + QString::fromStdString(receiver) + "' OR Sender = '" + QString::fromStdString(receiver) + "') AND Receiver != '" + QString::fromStdString(ALL) + "';";

    query_->exec(request);

    std::string messages;
    while (query_->next()) {
        auto sender = query_->value(0).toString();
        auto addressee = query_->value(1).toString();
        auto text = query_->value(2).toString();

        auto line = "<" + sender + "> -> <" + addressee + ">: " + text + "\n";
        messages.append(line.toStdString());
    }

    return messages;
}

std::list<std::string> Database::getLogins() const
{
    query_->exec("SELECT * FROM Users;");

    std::list<std::string> logins;
    while (query_->next()) {
        auto login = query_->value(0).toString();
        logins.push_back(login.toStdString());
    }
    return logins;
}

bool Database::removeUser(const std::string& login)
{
    if (login.empty()) {
        return false;
    }

    if (!isUserRegistered(login)) {
        return false;
    }

    auto request = "DELETE FROM Messages WHERE Receiver = '" + QString::fromStdString(login) + "';";
    query_->exec(request);

    request.clear();
    request = "DELETE FROM Users WHERE Login = '" + QString::fromStdString(login) + "';";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

bool Database::removeMessage(const std::string& message)
{
    if (message.empty()) {
        return false;
    }

    auto request = "DELETE FROM Messages WHERE Message = '" + QString::fromStdString(message) + "';";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

void Database::switchBanUser(const std::string& login)
{
    if (getIsBanned(login)) {
        auto request = "UPDATE Users SET Ban = false WHERE Login = '" + QString::fromStdString(login) + "';";
        query_->exec(request);
    }
    else {
        auto request = "UPDATE Users SET Ban = true WHERE Login = '" + QString::fromStdString(login) + "';";
        query_->exec(request);
    }

    emit updateDatabase();
}

QSqlDatabase* Database::getDatabase()
{
    return &database_;
}

bool Database::getIsBanned(const std::string& login) const
{
    if (login.empty() || !isUserRegistered(login)) {
        return true;
    }

    auto request = "SELECT Ban FROM Users WHERE Login = '" + QString::fromStdString(login) + "';";
    query_->exec(request);

    bool isBanned = true;
    if (query_->next()) {
        isBanned = query_->value(0).toBool();
    }
    return isBanned;
}

