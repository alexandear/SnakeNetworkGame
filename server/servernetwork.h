#pragma once

#include <QList>
#include <QTcpServer>

class QTcpSocket;
class ISocketAdapter;

class ServerNetwork : public QTcpServer
{
    Q_OBJECT

public:
    explicit ServerNetwork(quint16 t_port = static_cast<quint16>(0U));
    quint16 port() const { return serverPort(); }

signals:
    void received(const QString &t_message) const;
    void newClient(int t_id) const;

public slots:
    void sendAll(const QString &t_message) const;
    void sendOne(int t_id, const QString &t_message) const;
    void onNewConnection();
    void onDisconnected();

protected:
    QList<ISocketAdapter*> m_clients;

private:
    int m_id = 0;

};
