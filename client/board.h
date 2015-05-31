#pragma once

#include <QBasicTimer>
#include <QFrame>
#include <QDebug>
#include <QList>

#include "../common/snake.h"

class Board : public QFrame
{
    Q_OBJECT

public:
    const static int ALPHA;
    const static QColor HEAD_COLOR;
    const static QColor BODY_COLOR;
    const static QColor FOOD_COLOR;

    explicit Board(QWidget *t_parent = nullptr);

    int squareWidth() const { return rect().width() / m_dimensions.first; }
    int squareHeight() const { return rect().height() / m_dimensions.second; }
    int id() const { return m_id; }

signals:
    void directionChanged(Direction t_direction);

public slots:
    void setSnakes(const std::map<int, Snake> &t_snakes);
    void setTimerSignal();
    void setDimensions(const QPair<int, int> &t_dimensions);
    void setId(int t_id) { m_id = t_id; qDebug() << "Board::setId" << m_id; }
    void setWinner(int t_id);
    void setFood(const QList<QPoint> &t_food);
    void restart();
    
protected:
    void paintEvent(QPaintEvent *t_event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *t_event) Q_DECL_OVERRIDE;

private:

    QPair<int, int> m_dimensions = { 1, 1 };
    QList<QPoint> m_food;
    std::map<int, Snake> m_snakes;
    Direction m_direction = Direction::Up;
    int m_id = -1;
    bool m_gameOver = false;
    int m_winner = -1;
    bool m_flush = false;

    void drawSnake(QPainter &t_painter, const QColor &t_headColor, 
        const QColor &t_bodyColor, const Snake &t_snake);
    void drawSquare(QPainter &t_painter, const QColor &t_color, int t_x, int t_y);
    void drawWon(QPainter &t_painter);
    void flush(QPainter &t_painter);
};
