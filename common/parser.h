#pragma once

#include <QObject>
#include <map>

#include "snake.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    Parser() { }

    QString snakesToString(const std::map<int, Snake> &t_snakes) const;
    std::pair<int, Snake> snakeFromString(const QString &t_message) const;

    QString foodToString(const QList<QPoint> &t_food) const;
    QList<QPoint> foodFromString(const QString &t_message) const;

    QString scoresToString(const std::map<int, int> &t_scores) const;
    std::map<int, int> scoresFromString(const QString &t_message) const;

    QString directionToString(int t_id, Direction t_direction) const;
    std::pair<int, Direction> directionFromString(const QString &t_message) const;

    QString gameOverToString(int t_id) const;
    int gameOverFromString(const QString &t_message) const;

    QString dimensionsToString(const QPair<int, int> &t_dimensions) const;
    QPair<int, int> dimensionsFromString(const QString &t_message) const;

    QString idToString(int t_id) const;
    int idFromString(const QString &t_message) const;

public slots:
    void process(const QString &t_message) const;

signals:
    void directionChanged(const std::pair<int, Direction> &t_direction) const;
    void snakesChanged(const std::map<int, Snake> &t_snakes) const;
    void timerSignalChanged() const;
    void restartSignal() const;
    void dimensionsChanged(const QPair<int, int> &t_dimensions) const;
    void idChanged(int t_id) const;
    void gameOverSignal(int t_id) const;
    void foodAppeared(const QList<QPoint> &t_food) const;
    void scoresChanged(const std::map<int, int> &t_scores) const;

private:
    void processId(const QString &t_message) const;
    void processDirection(const QString &t_message) const;
    void processSnakes(const QString &t_message) const;
    void processRepaint(const QString &t_message) const;
    void processDimensions(const QString &t_message) const;
    void processGameOver(const QString &t_message) const;
    void processFood(const QString &t_message) const;
    void processScores(const QString &t_message) const;

};
