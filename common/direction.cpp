#include "direction.h"

QString toString(Direction t_direction)
{
    switch (t_direction)
    {
    case Direction::Up:
        return "Up";
    case Direction::Down:
        return "Down";
    case Direction::Left:
        return "Left";
    case Direction::Right:
        return "Right";
    default:
        return "Error";
        break;
    }
}


Direction fromString(const QString &t_direction)
{
    if (t_direction == "Up")
        return Direction::Up;
    else if (t_direction == "Down")
        return Direction::Down;
    else if (t_direction == "Left")
        return Direction::Left;
    else if (t_direction == "Right")
        return Direction::Right;
    else
        return Direction::Up;
}
