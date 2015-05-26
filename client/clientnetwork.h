#pragma once

#include <QObject>
#include <QHostAddress>

class ISocketAdapter;

class ClientNetwork : public QObject
{
    Q_OBJECT

public:
    explicit ClientNetwork(const QHostAddress &t_address, quint16 t_port,
        QObject *t_parent = nullptr);

signals:
    void received(const QString &t_message) const;
    void errorOccured(const QString &t_message) const;
    void connected() const;

public slots:
    void send(const QString &t_message) const;

private:
    QHostAddress m_address;
    quint16 m_port;

    ISocketAdapter *m_sock;
};
