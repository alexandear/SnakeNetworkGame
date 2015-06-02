#include <QtWidgets>

#include "settingswidget.h"

SettingsWidget::SettingsWidget()
{
    setupUi();

    connect(m_widthLineEdit, &QLineEdit::textChanged, this, &SettingsWidget::onWidthChanged);
    connect(m_heightLineEdit, &QLineEdit::textChanged, this, &SettingsWidget::onHeightChanged);
    connect(m_speedLineEdit, &QLineEdit::textChanged, this, &SettingsWidget::onSpeedChanged);

}

void SettingsWidget::onWidthChanged(const QString &t_widthStr) const
{
    emit widthChanged(t_widthStr.toInt());
}


void SettingsWidget::onHeightChanged(const QString &t_heightStr) const
{
    emit heightChanged(t_heightStr.toInt());
}


void SettingsWidget::onSpeedChanged(const QString &t_speedStr) const
{
    emit speedChanged(t_speedStr.toInt());
}


void SettingsWidget::onPenaltyChanged(const QString &t_penaltyStr) const
{
    emit penaltyChanged(t_penaltyStr.toInt());
}


void SettingsWidget::onWinScoreChanged(const QString &t_winScoreStr) const
{
    emit winScoreChanged(t_winScoreStr.toInt());
}


void SettingsWidget::setupUi()
{
    m_mainLayout = new QGridLayout;

    m_widthLabel = new QLabel(tr("Width [10, 30]:"));
    m_widthLineEdit = new QLineEdit;
    m_widthLineEdit->setValidator(new QIntValidator(10, 30, this));
    m_widthLineEdit->setText("10");

    m_mainLayout->addWidget(m_widthLabel, 0, 0, 1, 2);
    m_mainLayout->addWidget(m_widthLineEdit, 0, 4, 1, 4);

    m_heightLabel = new QLabel(tr("Height [20, 40]:"));
    m_heightLineEdit = new QLineEdit;
    m_heightLineEdit->setValidator(new QIntValidator(20, 40, this));
    m_heightLineEdit->setText("20");

    m_mainLayout->addWidget(m_heightLabel, 1, 0, 1, 2);
    m_mainLayout->addWidget(m_heightLineEdit, 1, 4, 1, 4);

    m_speedLabel = new QLabel(tr("Speed [50, 1000]:"));
    m_speedLineEdit = new QLineEdit;
    m_speedLineEdit->setValidator(new QIntValidator(50, 1000, this));
    m_speedLineEdit->setText("200");

    m_mainLayout->addWidget(m_speedLabel, 2, 0, 1, 2);
    m_mainLayout->addWidget(m_speedLineEdit, 2, 4, 1, 4);

    m_winScoreLabel = new QLabel(tr("Win score [5, 100]:"));
    m_winScoreLineEdit = new QLineEdit;
    m_winScoreLineEdit->setValidator(new QIntValidator(5, 100, this));
    m_winScoreLineEdit->setText("15");

    m_mainLayout->addWidget(m_winScoreLabel, 3, 0, 1, 2);
    m_mainLayout->addWidget(m_winScoreLineEdit, 3, 4, 1, 4);

    m_penaltyLabel = new QLabel(tr("Penalty [1, 100]:"));
    m_penaltyLineEdit = new QLineEdit;
    m_penaltyLineEdit->setValidator(new QIntValidator(1, 100, this));
    m_penaltyLineEdit->setText("5");

    setLayout(m_mainLayout);
}
