#include <QVector>
#include <QDataStream>

#include "socketadapter.h"

SocketAdapter::SocketAdapter(QObject *t_parent, QTcpSocket *t_sock)
    : ISocketAdapter(t_parent), m_msgSize(-1)
{
    if (!t_sock)
        m_tcpSocket = new QTcpSocket(this);
    else
        m_tcpSocket = t_sock;

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &SocketAdapter::onReadyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &SocketAdapter::onDisconnected);
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(onError(QAbstractSocket::SocketError)));
}


void SocketAdapter::onReadyRead()
{
    QString buff;
    QDataStream stream(m_tcpSocket);

    while(true) {
        if (m_msgSize < 0) {
            if (m_tcpSocket->bytesAvailable() < sizeof(int))
                return;
            stream >> m_msgSize;
        } else {
            if (m_tcpSocket->bytesAvailable() < m_msgSize)
                return;
            stream >> buff;
            emit message(buff);
            m_msgSize = -1;
        }
    }
}


void SocketAdapter::sendString(const QString& t_str) const
{
    QByteArray block;
    QDataStream sendStream(&block, QIODevice::ReadWrite);
    sendStream.setVersion(QDataStream::Qt_5_4);

    sendStream << quint16(0) << t_str;

    sendStream.device()->seek(0);
    sendStream << (quint16)(block.size() - sizeof(quint16));

    m_tcpSocket->write(block);
}


void SocketAdapter::onDisconnected() const
{
    m_tcpSocket->deleteLater();
    emit disconnected();
}


void SocketAdapter::onError(QTcpSocket::SocketError t_socketError) const
{
    QString errorStr;
    switch (t_socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        errorStr = tr("Server is shutdown.");
        break;
    case QAbstractSocket::HostNotFoundError:
        errorStr = tr("The host was not found. Please check the "
            "host name and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorStr = tr("The connection was refused by the peer. "
            "Make sure the server is running, "
            "and check the host name and port "
            "settings are correct.");
        break;
    default:
        errorStr = tr("The following error occurred: %1.").arg(m_tcpSocket->errorString());
    }
    emit ISocketAdapter::errorOccured(errorStr);
}
