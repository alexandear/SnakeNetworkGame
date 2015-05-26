#pragma once

#include <QString>

enum class Direction { Up, Down, Left, Right };

QString toString(Direction t_direction);
Direction fromString(const QString &t_direction);
