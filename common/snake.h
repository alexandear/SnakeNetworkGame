#pragma once

#include <QPoint>
#include <deque>
#include <iostream>
#include <QString>

#include "direction.h"

class Snake
{
public:

    Snake() { }
    Snake(int t_id, const std::deque<QPoint> &t_points);

    void update(Direction t_direction);
    void addToTail(const QPoint &t_point) { m_points.emplace_back(t_point); };
    void removeFromTail() { m_points.pop_back(); }

    void setPoints(const std::deque<QPoint> &t_points) { m_points = t_points; }
    const std::deque<QPoint> &points() const { return m_points; }
    std::deque<QPoint> &points() { return m_points; }
    int length() const { return static_cast<int>(m_points.size()); }
    int id() const { return m_id; }
    void setId(int t_id) { m_id = t_id; }
    const QPoint &head() const { return m_points.front(); }
    const QPoint &neck() const { return *(m_points.begin() + 1); }
    const QPoint &tail() const { return m_points.back(); }

    QString toString() const;
    static Snake fromString(const QString &t_message);

    bool operator!=(const Snake &lhs) { return m_points != lhs.m_points; }

private:
    int m_id = -1;
    std::deque<QPoint> m_points;
};
