#pragma once

#include <QObject>
#include <QPoint>
#include <QDebug>
#include <deque>


#include "../common/snake.h"
#include "../common/direction.h"
#include "../common/game.h"

class ClientNetwork;
class Board;

class ClientGame : public Game 
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
    void start() override { };
    void restart() override;

private:
    ClientNetwork *m_clientNetwork;
    Board *m_board;

    int m_id = -1;

};
