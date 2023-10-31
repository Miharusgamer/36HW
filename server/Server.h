#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include "RequestHandler.h"
#include "Database.h"

class Server: public QTcpServer{
  Q_OBJECT

  public:
    Server() = delete;
    Server(uint16_t port, Database* database);

  public slots:
    void incomingConnection(qintptr socketDescriptor);
    void readData();

  private:
    void sendToClient(QTcpSocket* socketClient, const QString& string);

    RequestHandler* handler_;
    QTcpSocket* socket_;
    QVector<QTcpSocket*> clientSockets_;
    QByteArray data_;
    quint16 incomingDataSize_;
};

#endif // SERVER_H
