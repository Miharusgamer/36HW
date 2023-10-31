#include "Server.h"
#include "RequestHandler.h"

Server::Server(uint16_t port, Database* database)
{
    this->listen(QHostAddress::Any, port);
    incomingDataSize_ = 0;
    handler_ = new RequestHandler(database);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::readData);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    clientSockets_.push_back(socket);
}

void Server::readData()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_5_6);
    if (input.status() == QDataStream::Ok){
        while (socket->bytesAvailable() > 0) {
            if (incomingDataSize_ == 0){
                if (socket->bytesAvailable() < 2){
                    break;
                }
                input >> incomingDataSize_;
            }
            if (socket->bytesAvailable() < incomingDataSize_){
                break;
            }
            QString commandArgs;
            int commandType;
            input >> commandType >> commandArgs;

            auto response = handler_->handle(commandType, commandArgs);
            sendToClient(socket, response);

            incomingDataSize_ = 0;
        }
    }
}

void Server::sendToClient(QTcpSocket* socketClient, const QString& string)
{
    QByteArray data;
    QDataStream output(&data, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_6);
    output << quint16(0) << string;
    output.device()->seek(0);
    output << quint16(data.size() - sizeof(quint16));
    socketClient->write(data);
}
