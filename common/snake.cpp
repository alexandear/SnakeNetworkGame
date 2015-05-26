#include <QDebug>

#include "snake.h"

Snake::Snake(int t_id, const std::deque<QPoint> &t_points)
    : m_id(t_id), m_points(t_points)
{
}


void Snake::update(Direction t_direction)
{
    const auto &head = m_points.front();
    int x = head.x();
    int y = head.y();
    m_points.pop_back();
    switch (t_direction)
    {
    case Direction::Up:
        --y;
        break;
    case Direction::Right:
        ++x;
        break;
    case Direction::Down: 
        ++y;
        break;
    case Direction::Left:
        --x;
        break;
    }
    m_points.emplace_front(x, y);
}


QString Snake::toString() const
{
    QString res = QString::number(m_id) + " ";
    for (auto const &point : m_points) {
        res += QString::number(point.x()) + " " + QString::number(point.y()) + " ";
    }
    res.remove(res.size() - 1, 1);
    return res;
}


Snake Snake::fromString(const QString &t_message)
{
    QStringList pointsStr = t_message.split(' ');
    std::deque<QPoint> points;
    for (int i = 1; i < pointsStr.size(); i += 2)
        points.emplace_back(pointsStr[i].toInt(), pointsStr[i + 1].toInt());
    auto id = pointsStr[0].toInt();
    return std::move(Snake(id, points));
}
