#pragma once

#include <QTcpSocket>

#include "isocketadapter.h"

class SocketAdapter : public ISocketAdapter
{
    Q_OBJECT

public:
    SocketAdapter(QObject *t_parent, QTcpSocket *t_sock = nullptr);
    virtual ~SocketAdapter() { }
    virtual void sendString(const QString &) const;

protected slots:
    void onReadyRead();
    void onDisconnected() const;
    void onError(QAbstractSocket::SocketError t_socketError) const;

protected:
    QTcpSocket *m_tcpSocket;
    qint16 m_msgSize;
};
