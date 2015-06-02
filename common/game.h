#pragma once

#include <QObject>

#include "../common/parser.h"

class Game : public QObject
{
    Q_OBJECT
public:
    virtual ~Game() { }

public slots:
    virtual void start() = 0;
    virtual void restart() = 0;

protected:
    Parser m_parser;
};

