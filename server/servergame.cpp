#include <QEvent>
#include <QDebug>

#include "servergame.h"
#include "servernetwork.h"
#include "../common/parser.h"
#include <functional>


ServerGame::ServerGame(quint16 t_port)
{
    try {
        m_serverNetwork = new ServerNetwork(t_port);
    } catch (const std::runtime_error &error) {
        throw error;
    }

    connect(m_serverNetwork, &ServerNetwork::received, &m_parser, &Parser::process);
    connect(m_serverNetwork, &ServerNetwork::newClient, this, &ServerGame::newClient);

    connect(&m_parser, &Parser::directionChanged, this, &ServerGame::setDirection);
    connect(this, &ServerGame::snakesChanged, &ServerGame::setSnakes);
    connect(this, &ServerGame::scoresChanged, std::bind(&ServerGame::updateScore, this, 0, 0));

    connect(this, &ServerGame::foodAppeared, this, &ServerGame::setFood);
}


void ServerGame::newClient(int t_id)
{
    qDebug() << "ServerGame::newClient";
    ++m_connectedClients;
    m_serverNetwork->sendOne(t_id, m_parser.idToString(t_id));
    m_serverNetwork->sendAll(m_parser.snakesToString(m_snakes));

    if (t_id == CLIENTS - 1) {
        m_allClientsConnected = true;
        emit allClientsConnected();
    }

}


void ServerGame::init()
{
    m_snakes.clear();
    m_directions.clear();
    m_food.clear();
    m_scores.clear();

    qDebug() << "ServerGame::init" << m_connectedClients;
    for (auto i = 0; i < m_connectedClients; ++i) {
        m_snakes[i] = startPosition(i);
        m_directions[i] = Direction::Up;
        m_scores[i] = 0;
    }

    changedDimensions(m_dimensions);
    emit snakesChanged(m_snakes);
    emit scoresChanged(m_scores);
    emit foodAppeared(generateFood());
    emit gameStarted();
}


void ServerGame::start()
{
    if (m_allClientsConnected) {
        qDebug() << "ServerGame::start";
        init();
        m_timer.stop();
        m_timer.start(m_gameSpeed, this);
    } else {
        emit allClientsNotConnected();
        qDebug() << "Not all clients connected";
    }
}


void ServerGame::restart()
{
    qDebug() << "ServerGame::restart";
    m_serverNetwork->sendAll("R");
    start();
}


void ServerGame::timerEvent(QTimerEvent *t_event)
{
    if (t_event->timerId() == m_timer.timerId()) {

        for (auto &snake : m_snakes) {
            snake.second.update(m_directions[snake.first]);

            if (checkWallCrash(snake.second)) {
                gameOver(determineWinner(snake.first));
                return;
            }

            if (checkNeckEated(snake.second)) {
                gameOver(snake.first);
                return;
            }

            if (checkOwnSnakesCrash(snake.second) 
                || checkOtherSnakesCrash(snake.second)) {
                updateScore(snake.second.id(), -m_penalty);
                gameOver(determineWinner());
                return;
            }

        }

        eatFood();
        eatTail();

        m_timerSignal = true;
        emit snakesChanged(m_snakes);
    }
}


bool ServerGame::checkNeckEated(const Snake &t_snake)
{
    for (auto const &snake : m_snakes) {
        if (snake.first != t_snake.id() 
            && t_snake.head() == snake.second.neck()) {
            return true;
        }
    }
    return false;
}


bool ServerGame::checkOwnSnakesCrash(const Snake &t_snake)
{
    auto res = find(t_snake.points().cbegin() + 1, t_snake.points().cend() - 1, t_snake.head());
    if (res != t_snake.points().cend() - 1) {
        return true;
    }
    return false;
}


bool ServerGame::checkOtherSnakesCrash(const Snake &t_snake) {
    for (auto const &snake : m_snakes) {
        auto other = snake.second.points();
        if (snake.first != t_snake.id()) {
            auto res = find(other.cbegin(), other.cend() - 1, t_snake.head());
            if (res != other.cend() - 1) {
                return true;
            }
        }
    }
    return false;
}


int ServerGame::determineWinner(int t_winner)
{
    for (auto const &snake : m_snakes)
        if (snake.second.id() != t_winner)
            return snake.second.id();
    return -1;
}


void ServerGame::setDirection(const std::pair<int, Direction> &t_direction) 
{
    if (m_timerSignal) {
        m_directions[t_direction.first] = t_direction.second;
        m_timerSignal = false;
    }
}


void ServerGame::setSnakes(const std::map<int, Snake> &t_snakes)
{
    m_snakes = t_snakes;
    const auto &snakesStr = m_parser.snakesToString(m_snakes);
    m_serverNetwork->sendAll(snakesStr);
}


void ServerGame::changedDimensions(const QPair<int, int> &t_dimensions)
{
    m_serverNetwork->sendAll(m_parser.dimensionsToString(t_dimensions));
}


Snake ServerGame::startPosition(int t_id)
{
    std::deque<QPoint> points({ { 3, height() - 3 }, { 3, height() - 2 }, { 3, height() - 1 } });
    for (auto & point : points)
        point.setX(point.x() + 4 * t_id);
    return std::move(Snake(t_id, points));
}


quint16 ServerGame::port() const
{
    return m_serverNetwork->port();
}


int ServerGame::determineWinner()
{
    auto max = std::max_element(m_scores.begin(), m_scores.end(),
        m_scores.value_comp());
    return max != m_scores.end() ? max->second : 0;
}


bool ServerGame::checkWallCrash(const Snake &t_snake)
{
    for (auto const &point : t_snake.points()) {
        if (point.x() < 0 || point.x() >= width()
            || point.y() < 0 || point.y() >= height()) {
            return true;
        }
    }
    
    return false;
}


void ServerGame::gameOver(int t_id)
{
    m_timer.stop();
    m_serverNetwork->sendAll(m_parser.gameOverToString(t_id));
}


QList<QPoint> ServerGame::generateFood()
{
    qDebug() << "ServerGame::generateFood";
    QList<QPoint> food;

    for (std::size_t i = 0; i < m_snakes.size(); ++i) {
        do {
            auto x = qrand() % width();
            auto y = qrand() % height();
            if (!isGameHasPoint({ x, y })) {
                food.push_back({ x, y });
                break;
            }
        } while (true);
    }
    return food;
}


bool ServerGame::isGameHasPoint(const QPoint &t_point) const
{
    for (auto const &snake : m_snakes) {
        for (auto const &point : snake.second.points()) {
            if (point == t_point)
                return true;
        }
    }
    return false;
}


void ServerGame::setFood(const QList<QPoint> &t_food)
{
    m_food = t_food;
    m_serverNetwork->sendAll(m_parser.foodToString(t_food));
}


void ServerGame::eatFood()
{
    for (auto &snake : m_snakes) {
        for (auto const &food : m_food) {
            if (snake.second.head() == food) {
                snake.second.addToTail(food);
                updateScore(snake.first, 1);
                emit foodAppeared(generateFood());
                return;
            }
        }
    }
}


void ServerGame::eatTail()
{
    for (auto &left : m_snakes) {
        for (auto &right : m_snakes) {
            if (left.second != right.second 
                && left.second.head() == right.second.tail()) {

                left.second.addToTail(left.second.tail());
                updateScore(left.first, 1);
                right.second.removeFromTail();
                updateScore(right.first, -1);

            }
        }
    }
}


void ServerGame::updateScore(int t_id, int t_score)
{
    m_scores[t_id] += t_score;
    m_serverNetwork->sendAll(m_parser.scoresToString(m_scores));
    checkMaxScoreReached();
}


bool ServerGame::checkMaxScoreReached() {
    for (auto const &score : m_scores) {
        if (score.second >= m_winScore)
            gameOver(score.first);
    }
    return true;
}
