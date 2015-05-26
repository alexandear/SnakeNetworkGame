#include <QKeyEvent>
#include <QPainter>

#include "board.h"

const int Board::ALPHA = 150;
const QColor Board::HEAD_COLOR = 0xFD7400;
const QColor Board::BODY_COLOR = 0xFFE11A;
const QColor Board::FOOD_COLOR = 0x004358;

Board::Board(QWidget *t_parent) : QFrame(t_parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
}


void Board::paintEvent(QPaintEvent *t_event)
{
    QFrame::paintEvent(t_event);

    QPainter painter(this);

    if (m_gameOver) {
        drawWon(painter);
        return;
    }

    if (m_flush)
        flush(painter);

    for (auto const &client : m_snakes) {
        auto bodyColor = BODY_COLOR;
        auto headColor = HEAD_COLOR;
        if (client.second.id() != m_id) {
            bodyColor.setAlpha(ALPHA);
            headColor.setAlpha(ALPHA);
        }
        drawSnake(painter, bodyColor, headColor, client.second);
    }

    for (auto const &point : m_food) {
        drawSquare(painter, FOOD_COLOR, point.x() * squareWidth(), point.y() * squareHeight());
    }


}


void Board::drawSnake(QPainter &t_painter, const QColor &t_headColor, 
    const QColor &t_bodyColor, const Snake &t_snake)
{
    auto const &points = t_snake.points();
    for (auto it = points.cbegin() + 1; it != points.cend(); ++it)
        drawSquare(t_painter, t_bodyColor, it->x() * squareWidth(), it->y() * squareHeight());
    drawSquare(t_painter, t_headColor, points.cbegin()->x() * squareWidth(),
        points.cbegin()->y() * squareHeight());
}


void Board::drawSquare(QPainter &t_painter, const QColor &t_color, int t_x, int t_y)
{
    t_painter.fillRect(t_x + 1, t_y + 1, squareWidth() - 2, squareHeight() - 2, t_color);

    t_painter.setPen(t_color.light());
    t_painter.drawLine(t_x, t_y + squareHeight() - 1, t_x, t_y);
    t_painter.drawLine(t_x, t_y, t_x + squareWidth() - 1, t_y);

    t_painter.setPen(t_color.dark());
    t_painter.drawLine(t_x + 1, t_y + squareHeight() - 1, t_x + squareWidth() - 1, t_y + squareHeight() - 1);
    t_painter.drawLine(t_x + squareWidth() - 1, t_y + squareHeight() - 1, t_x + squareWidth() - 1, t_y + 1);

}


void Board::drawWon(QPainter &t_painter)
{
    auto text = tr("You lose");
    if (m_winner == m_id)
        text = tr("You won!");
    auto font = t_painter.font();
    font.setPointSize(font.pointSize() * 2);
    t_painter.setFont(font);

    t_painter.setPen(QColor());
    t_painter.drawText(width() / 2 - 10, height() / 2, text);
}


void Board::keyPressEvent(QKeyEvent *t_event)
{
    qDebug() << "keyPressEvent";
    if (!m_timerSignal)
        return;

    switch (t_event->key()) {
    case Qt::Key_A:
    case Qt::Key_Left:
        if (m_direction != Direction::Right) {
            m_direction = Direction::Left;
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if (m_direction != Direction::Left)
            m_direction = Direction::Right;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if (m_direction != Direction::Up)
            m_direction = Direction::Down;
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        if (m_direction != Direction::Down)
            m_direction = Direction::Up;
        break;
    default:
        QFrame::keyPressEvent(t_event);
    }
    m_timerSignal = false;
    emit directionChanged(m_direction);
}


void Board::setSnakes(const std::map<int, Snake> &t_snakes)
{
    qDebug() << "Board::setSnakes" << m_snakes.size();
    m_snakes = t_snakes;
    update();
}


void Board::setTimerSignal()
{
    qDebug() << "Board::setTimerSignal";
    m_timerSignal = true;
    update();
}


void Board::setDimensions(const QPair<int, int> &t_dimensions)
{
    qDebug() << "Board::setDimensions" << t_dimensions.first << t_dimensions.second;
    m_dimensions = t_dimensions;
}


void Board::setWinner(int t_id) {
    qDebug() << "Board::setWinner" << t_id;
    m_winner = t_id;
    m_gameOver = true;
    update();
}


void Board::setFood(const QList<QPoint> &t_food)
{
    qDebug() << "Board::setFood";
    m_food = t_food;
    update();
}


void Board::restart()
{
    m_flush = true;
    m_gameOver = false;
    m_direction = Direction::Up;
}


void Board::flush(QPainter &t_painter)
{
    t_painter.setCompositionMode(QPainter::CompositionMode_Clear);
    t_painter.fillRect(0, 0, width(), height(), QPainter::CompositionMode_Clear);
    m_flush = false;
    update();
}
