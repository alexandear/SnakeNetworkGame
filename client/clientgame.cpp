#include "clientgame.h"
#include "clientnetwork.h"
#include "../common/parser.h"
#include "board.h"

ClientGame::ClientGame(Board *t_board, const QString &t_address, quint16 t_port)
    : m_clientNetwork(new ClientNetwork(QHostAddress(t_address), t_port)), 
    m_board(t_board)
{
    connect(m_clientNetwork, &ClientNetwork::connected, this, &ClientGame::connected);
    connect(m_clientNetwork, &ClientNetwork::errorOccured, this, &ClientGame::connectionError);
    connect(m_clientNetwork, &ClientNetwork::received, &m_parser, &Parser::process);

    connect(m_board, &Board::directionChanged, this, &ClientGame::changeDirection);

    connect(&m_parser, &Parser::idChanged, this, &ClientGame::setId);
    connect(&m_parser, &Parser::restartSignal, this, &ClientGame::restart);
    connect(&m_parser, &Parser::scoresChanged, this, &ClientGame::scoresChanged);
    connect(&m_parser, &Parser::foodAppeared, m_board, &Board::setFood);
    connect(&m_parser, &Parser::snakesChanged, m_board, &Board::setSnakes);
    connect(&m_parser, &Parser::dimensionsChanged, m_board, &Board::setDimensions);
    connect(&m_parser, &Parser::gameOverSignal, m_board, &Board::setWinner);
}


void ClientGame::restart()
{
    m_board->restart();
}


void ClientGame::changeDirection(Direction t_direction) const
{
    const auto &str = m_parser.directionToString(m_id, t_direction);
    qDebug() << "ClientGame::on_direction" << str;
    m_clientNetwork->send(str);
}


void ClientGame::setId(int t_id)
{
    qDebug() << "ClientGame::setId" << t_id;
    m_id = t_id;
    m_board->setId(m_id);
}
