#pragma once

#include <QBasicTimer>
#include <QObject>
#include <QDebug>
#include <QtNetwork>
#include <QList>

#include "../common/snake.h"
#include "../common/game.h"

class ServerNetwork;

class ServerGame : public Game
{
    Q_OBJECT

public:

    ServerGame(quint16 t_port);
    quint16 port() const;

    const quint16 CLIENTS = 2;
    int width() const { return m_dimensions.first; }
    int height() const { return m_dimensions.second; }

public slots:
    void start() override;
    void restart() override;
    void setDirection(const std::pair<int, Direction> &t_direction);
    void setSnakes(const std::map<int, Snake> &t_snakes);
    void setFood(const QList<QPoint> &t_food);
    void changedDimensions(const QPair<int, int> &t_dimensions);
    void newClient(int t_id);
    void gameOver(int t_id);

    void setWidth(int t_value) { m_dimensions.second = t_value; }
    void setHeight(int t_value) { m_dimensions.second = t_value; };
    void setSpeed(int t_value) { m_gameSpeed = t_value; }
    void setPenalty(int t_value) { m_penalty = t_value; }
    void setWinScore(int t_value) { m_winScore = t_value; }

signals:
    void snakesChanged(const std::map<int, Snake> &t_snakes);
    void allClientsConnected();
    void allClientsNotConnected();
    void gameStarted();
    void snakeWallCrashed(int t_id);
    void foodAppeared(const QList<QPoint> &t_food);
    void scoresChanged(const std::map<int, int> &t_scores);

protected:
    void timerEvent(QTimerEvent *t_event) Q_DECL_OVERRIDE;

private:
    quint16 m_gameSpeed = 200;
    QPair<int, int> m_dimensions = { 10 , 20 };
    quint16 m_winScore = 15;
    int m_penalty = 5;

    quint16 m_connectedClients = 0;
    bool m_allClientsConnected = false;
    ServerNetwork *m_serverNetwork;

    std::map<int, int> m_scores;
    std::map<int, Snake> m_snakes;
    std::map<int, Direction> m_directions;
    QList<QPoint> m_food;

    QBasicTimer m_timer;
    bool m_timerSignal = false;

    void init();
    Snake startPosition(int t_id);
    bool checkWallCrash(const Snake &t_snake);
    bool checkMaxScoreReached();
    bool checkOwnSnakesCrash(const Snake &t_snake);
    bool checkOtherSnakesCrash(const Snake &t_snake);
    bool checkNeckEated(const Snake &t_snake);
    QList<QPoint> generateFood();

    bool isGameHasPoint(const QPoint &t_point) const;
    void eatFood();
    void eatTail();
    void updateScore(int t_id, int t_score);
    int determineWinner();
    int determineWinner(int t_loser);
};
