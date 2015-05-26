#pragma once

#include <QTcpSocket>

#include "../common/socketadapter.h"

class ServerSocketAdapter : public SocketAdapter
{
    Q_OBJECT

public:
    explicit ServerSocketAdapter(QTcpSocket* t_sock, QObject *t_parent = nullptr)
        : SocketAdapter(t_parent, t_sock)
    {
    }
};
