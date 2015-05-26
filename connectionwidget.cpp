#include <QtWidgets>
#include <QtNetwork>

#include "connectionwidget.h"


ConnectionWidget::ConnectionWidget(QWidget *t_parent) : QWidget(t_parent)
{
    setupUi();

    connect(m_hostCombo, &QComboBox::editTextChanged, this, &ConnectionWidget::enableConnectButton);
    connect(m_portLineEdit, &QLineEdit::textChanged, this, &ConnectionWidget::enableConnectButton);
    connect(m_connectButton, &QPushButton::clicked, this, &ConnectionWidget::onConnectButtonClicked);

    enableConnectButton();
}


void ConnectionWidget::setupUi()
{

    m_hostLabel = new QLabel(tr("&Server:"));
    m_portLabel = new QLabel(tr("&Port:"));

    m_hostCombo = new QComboBox;
    m_hostCombo->setEditable(true);

    setupConnectionCombo();

    m_portLineEdit = new QLineEdit;
    m_portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    m_portLineEdit->setText("1024");

    m_hostLabel->setBuddy(m_hostCombo);
    m_portLabel->setBuddy(m_portLineEdit);

    m_connectButton = new QPushButton(tr("Connect"));

    m_mainLayout = new QGridLayout;
    m_mainLayout->addWidget(m_hostLabel, 0, 0);
    m_mainLayout->addWidget(m_hostCombo, 0, 1);
    m_mainLayout->addWidget(m_portLabel, 1, 0);
    m_mainLayout->addWidget(m_portLineEdit, 1, 1);
    m_mainLayout->addWidget(m_connectButton, 2, 0, 1, 2);

    setLayout(m_mainLayout);
}


void ConnectionWidget::setupConnectionCombo()
{
    auto name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        m_hostCombo->addItem(name);
        auto domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            m_hostCombo->addItem(name + QChar('.') + domain);
    }
    auto localhost = QHostAddress(QHostAddress::LocalHost).toString();
    if (name != localhost)
        m_hostCombo->addItem(localhost);

    // find out IP addresses of this machine
    auto ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (auto const &address : ipAddressesList) {
        if (!address.isLoopback())
            m_hostCombo->addItem(address.toString());
    }
    // add localhost addresses
    for (auto const &address : ipAddressesList) {
        if (address.isLoopback())
            m_hostCombo->addItem(address.toString());
    }

    m_hostCombo->setCurrentIndex(m_hostCombo->findText(localhost));
}


void ConnectionWidget::enableConnectButton()
{
    m_connectButton->setEnabled(!m_hostCombo->currentText().isEmpty()
        && !m_portLineEdit->text().isEmpty());
}


void ConnectionWidget::onConnectButtonClicked()
{
    emit connectClicked(m_hostCombo->currentText(), m_portLineEdit->text().toInt());
}


void ConnectionWidget::onConnectEstablished()
{
    m_connectButton->setEnabled(false);
}
