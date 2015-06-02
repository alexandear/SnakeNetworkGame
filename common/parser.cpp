#include <cstring>
#include <QDebug>
#include <QString> 

#include "parser.h"

/*
 * Protocol
 *
 * From clients:
 * Direction: "O2Up", 2 - id, Up - orientation (Up, Down, Left, Right)
 *
 * From server:
 * Timer: "T"
 * Id: "I5", 5 - id number
 * Dimensions: "D 15 20", 15 - width, 20 - height
 * Snakes: "S1 2 4 2 5 2 6; S2 3 10 3 11 3 12", 1 - client id, 2 4 - points x y in snake
 * GameStatus: "G1" - game over, 1 - loser
 * Food: "F 2 4 5 6"
 * Scores: "C1 5 C2 10", 1 - id
 *
 */
void Parser::process(const QString &t_message) const
{
    if (t_message.isEmpty())
        return;

    auto ch = t_message[0];
    auto rightStr = t_message.right(t_message.size() - 1);
    if (ch == 'I')
        processId(rightStr);
    else if (ch == 'O')
        processDirection(rightStr);
    else if (ch == 'D')
        processDimensions(rightStr);
    else if (ch == 'F')
        processFood(rightStr);
    else if (ch == 'S')
        processSnakes(t_message);
    else if (ch == 'G')
        processGameOver(rightStr);
    else if (ch == 'C')
        processScores(t_message);
    else if (ch == 'R')
        emit restartSignal();
}


void Parser::processId(const QString &t_message) const
{
    emit idChanged(idFromString(t_message));
}


QString Parser::directionToString(int t_id, Direction t_direction) const
{
    return "O" + QString::number(t_id) + ::toString(t_direction);
}


QString Parser::idToString(int t_id) const
{
    return "I" + QString::number(t_id);
}


int Parser::idFromString(const QString &t_message) const
{
    return t_message.toInt();
}


std::pair<int, Direction> Parser::directionFromString(const QString &t_message) const
{
    const auto &directionStr = t_message.right(t_message.size() - 1);
    auto id = QString(t_message[0]).toInt();
    return{ id, ::fromString(directionStr) };
}


void Parser::processDirection(const QString &t_message) const
{
    emit directionChanged(directionFromString(t_message));
}


void Parser::processSnakes(const QString &t_message) const
{
    qDebug() << "Parser::processSnakes" << t_message;
    QStringList snakesStr = t_message.split(';');
    snakesStr.removeLast();
    std::map<int, Snake> snakes;
    for (auto const & item : snakesStr)
        snakes.emplace(snakeFromString(item));
    emit snakesChanged(snakes);
}


QString Parser::snakesToString(const std::map<int, Snake> &t_snakes) const
{
    QString res;
    QTextStream stream(&res);
    for (auto const &snake : t_snakes)
        stream << "S" << snake.first << " " << snake.second.toString() << ";";
    return std::move(res);

}


std::pair<int, Snake> Parser::snakeFromString(const QString &t_message) const
{
    auto id = QStringRef(&t_message, 1, 1).toInt();
    auto snake = Snake::fromString(QStringRef(&t_message, 3, t_message.size() - 3).toString());
    qDebug() << snake.toString();
    return { id, std::move(snake) };
}


QString Parser::foodToString(const QList<QPoint> &t_food) const
{
    qDebug() << "Parser::foodToString";
    QString res;
    QTextStream stream(&res);
    stream << "F";
    for (auto const &point : t_food)
        stream << point.x() << " " << point.y() << " ";

    return res;
}


QList<QPoint> Parser::foodFromString(const QString &t_message) const
{
    qDebug() << "Parser::foodFromString" << t_message;
    QList<QPoint> res;
    QStringList foodStr = t_message.split(' ');
    foodStr.removeLast();
    for (auto i = 0; i < foodStr.size(); i += 2)
        res.push_back({ foodStr[i].toInt(), foodStr[i + 1].toInt() });

    return std::move(res);
}


void Parser::processDimensions(const QString &t_message) const
{
    emit dimensionsChanged(dimensionsFromString(t_message));
}


void Parser::processGameOver(const QString &t_message) const
{
    emit gameOverSignal(gameOverFromString(t_message));
}


void Parser::processFood(const QString &t_message) const
{
    emit foodAppeared(foodFromString(t_message));
}


void Parser::processScores(const QString &t_message) const
{
    emit scoresChanged(scoresFromString(t_message));
}


QString Parser::scoresToString(const std::map<int, int> &t_scores) const
{
    qDebug() << "Parser::scoresToString";
    QString res;
    QTextStream stream(&res);
    for (auto const &score : t_scores)
        stream << "C" << score.first << " " << score.second << " ";
    return std::move(res);
}


std::map<int, int> Parser::scoresFromString(const QString &t_message) const
{
    qDebug() << "Parser::scoresFromString";
    QStringList strList = t_message.split(' ');
    strList.removeLast();
    std::map<int, int> scores;
    for (int i = 0; i < strList.size(); i += 2)
        scores.emplace(QString(strList[i][1]).toInt(), strList[i + 1].toInt());
    return std::move(scores);
}


QString Parser::gameOverToString(int t_id) const
{
    return "G" + QString::number(t_id);
}


int Parser::gameOverFromString(const QString &t_message) const
{
    return t_message.toInt();
}


QString Parser::dimensionsToString(const QPair<int, int> &t_dimensions) const
{
    return "D " + QString::number(t_dimensions.first)
        + " " + QString::number(t_dimensions.second);
}


QPair<int, int> Parser::dimensionsFromString(const QString &t_message) const
{
    QStringList dimensionsStr = t_message.split(' ');
    dimensionsStr.removeFirst();
    int first = dimensionsStr[0].toInt();
    int second = dimensionsStr[1].toInt();
    return{ first, second };
}
