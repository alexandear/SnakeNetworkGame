#pragma once

#include <QObject>

class ISocketAdapter : public QObject 
{
    Q_OBJECT

public:
    explicit ISocketAdapter(QObject *t_parent) : QObject(t_parent) { }
    virtual ~ISocketAdapter() { }
    virtual void sendString(const QString &t_message) const = 0;

signals:
    void message(const QString &t_message) const;
    void errorOccured(const QString &t_error) const;
    void disconnected() const;
};
