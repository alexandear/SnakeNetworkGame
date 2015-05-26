#include <QVector>
#include <QTcpSocket>
#include <QDataStream>

#include "clientsocketadapter.h"

ClientSocketAdapter::ClientSocketAdapter(const QHostAddress &t_address, quint16 t_port,
    QObject *t_parent)
  : SocketAdapter(t_parent)
{
    connect(m_tcpSocket, &QTcpSocket::connected, this, &ClientSocketAdapter::onConnected);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &ClientSocketAdapter::connected);
    m_tcpSocket->connectToHost(t_address, t_port);
}


void ClientSocketAdapter::onConnected()
{
    qDebug() << "Connected to host";
}
