#include <QDebug>

#include "clientnetwork.h"
#include "clientsocketadapter.h"

ClientNetwork::ClientNetwork(const QHostAddress &t_address, quint16 t_port)
    : m_address(t_address), m_port(t_port), 
    m_sock(new ClientSocketAdapter(m_address, m_port, this))
{
    connect(m_sock, &ClientSocketAdapter::message, this, &ClientNetwork::received);
    connect(m_sock, &ClientSocketAdapter::errorOccured, this, &ClientNetwork::errorOccured);
    connect(m_sock, SIGNAL(connected()), this, SIGNAL(connected()));
}


void ClientNetwork::send(const QString &t_message) const
{
    m_sock->sendString(t_message);
}
