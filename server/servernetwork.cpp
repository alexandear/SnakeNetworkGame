#include <QTcpSocket>
#include <QString>
#include <stdexcept>

#include "servernetwork.h"
#include "serversocketadapter.h"

ServerNetwork::ServerNetwork(quint16 t_port)
{
    connect(this, &ServerNetwork::newConnection, this, &ServerNetwork::onNewConnection);

    if (!listen(QHostAddress::Any, t_port)) {
        close();
        throw std::runtime_error("Server coudn't started. Error:" + errorString().toStdString());
    }

    qDebug() << "Server started on port" << serverPort();
}


void ServerNetwork::onNewConnection()
{
    qDebug() << "ServerNetwork::onNewconnection" << m_id;

    auto clientSock = nextPendingConnection();
    auto sockHandle = new ServerSocketAdapter(clientSock);

    m_clients.push_back(sockHandle);
    emit newClient(m_id);
    ++m_id;

    connect(sockHandle, &ISocketAdapter::disconnected, this, &ServerNetwork::onDisconnected);
    connect(sockHandle, &ISocketAdapter::message, this, &ServerNetwork::sendAll);
    connect(sockHandle, &ISocketAdapter::message, this, &ServerNetwork::received);
}


void ServerNetwork::onDisconnected()
{
    qDebug() << "ServerNetwork::onDisconnected" << endl;
    auto client = static_cast<ServerSocketAdapter*>(sender());
    m_clients.removeOne(client);
    delete client;
}


void ServerNetwork::sendOne(int t_id, const QString &t_message) const
{
    qDebug() << "ServerNetwork::sendOne" << t_message;
    m_clients[t_id]->sendString(t_message);
}


void ServerNetwork::sendAll(const QString &t_message) const
{
    qDebug() << "ServerNetwork::sendAll" << t_message;
    for (auto *sock : m_clients)
        sock->sendString(t_message);
}
