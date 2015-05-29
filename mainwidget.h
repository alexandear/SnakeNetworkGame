#pragma once

#include <QWidget>
#include "common/snake.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QRadioButton;
class QPushButton;
class QVBoxLayout;
class QDialogButtonBox;
class QGridLayout;
class QStatusBar;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class Game;
class Board;
class ClientGame;
class ServerGame;
class ConnectionWidget;
class SettingsWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *t_parent = nullptr);

public slots:

private:
    Board *m_board;
    Game *m_client;
    Game *m_server;

    QPushButton *m_playButton;
    QRadioButton *m_serverRadioButton;
    QRadioButton *m_clientRadioButton;
    QLCDNumber *m_scoreServer;
    QLCDNumber *m_scoreClient;
    QLabel *m_portLabel;
    QLineEdit *m_portLineEdit;
    QVBoxLayout *m_rightPanelLayout;
    QGridLayout *m_mainLayout;
    QStatusBar *m_statusBar;
    ConnectionWidget *m_connectionWidget;
    SettingsWidget *m_settingsWidget;

    void setupUi();
    void setupServerUi();

    void enableConnectButton();

    void createClient(const QString &t_address, quint16 t_port);

private slots:
    void initServer();
    void initClient();
    void displayClientConnectionError(const QString &t_message);
    void displayServerConnectionError(const QString &t_message);
    void onConnectClicked(const QString &t_address, quint16 t_port);
    void onGameStarted();

    void connectedToServer();
    void clientConnected();
    void updateScores(const std::map<int, int> &t_scores);
    
};
