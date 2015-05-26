#include <QtWidgets>
#include <functional>

#include "mainwidget.h"
#include "client/board.h"
#include "client/clientgame.h"
#include "server/servergame.h"
#include "connectionwidget.h"
#include "settingswidget.h"


MainWidget::MainWidget(QWidget *t_parent)
    : QWidget(t_parent), m_board(new Board)
{
    setFocusPolicy(Qt::StrongFocus);
    setupUi();
    connect(m_serverRadioButton, &QRadioButton::toggled, this, &MainWidget::initServer);
    connect(m_clientRadioButton, &QRadioButton::toggled, this, &MainWidget::initClient);
}


void MainWidget::initServer()
{
    m_serverRadioButton->setEnabled (false);
    m_clientRadioButton->setEnabled(false);
    m_portLabel->setEnabled(false);
    m_portLineEdit->setEnabled(false);
    auto port = m_portLineEdit->text().toInt();

    try {
        m_server = new ServerGame(port);
    } catch (std::runtime_error &error) {
        displayServerConnectionError(error.what());
        return;
    }

    setupServerUi();

    connect(m_settingsWidget, &SettingsWidget::widthChanged, m_server, &ServerGame::setWidth);
    connect(m_settingsWidget, &SettingsWidget::heightChanged, m_server, &ServerGame::setHeight);
    connect(m_settingsWidget, &SettingsWidget::speedChanged, m_server, &ServerGame::setSpeed);
    connect(m_settingsWidget, &SettingsWidget::penaltyChanged, m_server, &ServerGame::setPenalty);
    connect(m_settingsWidget, &SettingsWidget::winScoreChanged, m_server, &ServerGame::setWinScore);

    connect(m_server, &ServerGame::allClientsConnected, this, &MainWidget::clientConnected);
    connect(m_server, &ServerGame::allClientsNotConnected, [&](){ 
        m_statusBar->showMessage(tr("Coudn't start game. Not all clients already connected.")); 
    });
    connect(m_server, &ServerGame::gameStarted, this, &MainWidget::onGameStarted);

    createClient("127.0.0.1", m_server->port());
}


void MainWidget::setupServerUi()
{
    m_settingsWidget = new SettingsWidget;
    m_rightPanelLayout->addWidget(m_settingsWidget);

    m_rightPanelLayout->addWidget(m_playButton);
    connect(m_playButton, &QPushButton::clicked, m_server, &ServerGame::start);

    setWindowTitle(windowTitle() + tr(" - Server"));
}


void MainWidget::initClient()
{
    setWindowTitle(windowTitle() + tr(" - Client"));
    m_connectionWidget = new ConnectionWidget;
    m_rightPanelLayout->addWidget(m_connectionWidget);

    connect(m_connectionWidget, &ConnectionWidget::connectClicked, this, &MainWidget::onConnectClicked);
}


void MainWidget::setupUi()
{
    m_playButton = new QPushButton(tr("Play"));
    m_serverRadioButton = new QRadioButton(tr("Server"));
    m_clientRadioButton = new QRadioButton(tr("Client"));

    m_scoreServer = new QLCDNumber(4);
    m_scoreClient = new QLCDNumber(4);
    
    m_rightPanelLayout = new QVBoxLayout;

    m_portLabel = new QLabel(tr("Port"));
    m_portLineEdit = new QLineEdit;
    m_portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    auto hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(m_serverRadioButton);
    hBoxLayout->addWidget(m_portLabel);
    hBoxLayout->addWidget(m_portLineEdit);
    m_portLineEdit->setText("1024");
    m_rightPanelLayout->addLayout(hBoxLayout);
    m_rightPanelLayout->addWidget(m_scoreServer);

    m_rightPanelLayout->addSpacing(10);
    m_rightPanelLayout->addWidget(m_clientRadioButton);
    m_rightPanelLayout->addWidget(m_scoreClient);
    
    m_statusBar = new QStatusBar;
    m_statusBar->showMessage(tr("Waiting for connection..."));

    m_mainLayout = new QGridLayout;
    m_mainLayout->addWidget(m_board, 0, 0, 14, 12);
    m_mainLayout->addLayout(m_rightPanelLayout, 0, 12, 4, 8);
    m_mainLayout->addWidget(m_statusBar, 15, 0, 1, 20);

    setLayout(m_mainLayout);

    setWindowTitle(tr("Snake Network Game"));
    resize(630, 595);
}


void MainWidget::displayClientConnectionError(const QString &t_message)
{
    QMessageBox::critical(this, tr("Client"), t_message);
}


void MainWidget::displayServerConnectionError(const QString &t_message)
{
    QMessageBox::critical(this, tr("Server"), t_message);
}


void MainWidget::createClient(const QString &t_address, quint16 t_port)
{
    m_client = new ClientGame(m_board, t_address, t_port);
    connect(m_client, &ClientGame::connectionError, 
        this, &MainWidget::displayClientConnectionError);
    connect(m_client, &ClientGame::scoresChanged, this, &MainWidget::updateScores);
}


void MainWidget::onConnectClicked(const QString &t_address, quint16 t_port)
{
    createClient(t_address, t_port);
    connect(m_client, &ClientGame::connected, this, &MainWidget::connectedToServer);
    connect(m_client, &ClientGame::connected, m_connectionWidget, &ConnectionWidget::onConnectEstablished);
}


void MainWidget::connectedToServer()
{
    m_statusBar->showMessage(tr("Connected to server"));
}


void MainWidget::clientConnected()
{
    m_statusBar->showMessage(tr("Client connected"));
}


void MainWidget::onGameStarted()
{
    m_playButton->setText(tr("Restart"));
    disconnect(m_playButton, &QPushButton::clicked, m_server, &ServerGame::start);
    connect(m_playButton, &QPushButton::clicked, m_server, &ServerGame::restart);
    m_board->setFocus();
}


void MainWidget::updateScores(const std::map<int, int> &t_scores)
{
    qDebug() << "MainWidget::updateScore" << t_scores.at(0);
    m_scoreServer->display(t_scores.at(0));
    try {
        m_scoreClient->display(t_scores.at(1));
    } catch (std::out_of_range e) { }
}
