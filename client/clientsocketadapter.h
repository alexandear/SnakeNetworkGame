#pragma once

#include <QHostAddress>

#include "../common/socketadapter.h"

class ClientSocketAdapter : public SocketAdapter
{
    Q_OBJECT

public:
    ClientSocketAdapter(const QHostAddress &t_address, quint16 t_port,
        QObject *t_parent);

signals:
    void connected();

public slots:
    void onConnected();
};
