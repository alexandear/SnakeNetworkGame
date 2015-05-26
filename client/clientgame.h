#pragma once

#include <QObject>
#include <QPoint>
#include <deque>

#include "../common/snake.h"
#include "../common/direction.h"

class ClientNetwork;
class Parser;
class Board;

class ClientGame : public QObject
{
    Q_OBJECT

public:
    ClientGame(Board *t_board, const QString &t_address, quint16 t_port);

signals:
    void connectionError(const QString &t_message) const;
    void connected() const;
    void scoresChanged(const std::map<int, int> &t_scores) const;

public slots:
    void changeDirection(Direction t_direction) const;
    void setId(int t_id);

private:
    ClientNetwork *m_clientNetwork;
    Parser *m_parser;
    Board *m_board;

    int m_id = -1;

};
