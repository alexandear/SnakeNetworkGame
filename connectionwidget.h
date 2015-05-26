#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    ConnectionWidget(QWidget *t_parent = nullptr);

signals:
    void connectClicked(const QString &t_address, quint16 t_port) const;

public slots:
    void onConnectEstablished();

private:
    QComboBox *m_hostCombo;
    QGridLayout *m_mainLayout;
    QPushButton *m_connectButton;
    QLabel *m_hostLabel;
    QLabel *m_portLabel;
    QLineEdit *m_portLineEdit;

    void setupUi();
    void setupConnectionCombo();
    void enableConnectButton();

private slots:
    void onConnectButtonClicked();
};

