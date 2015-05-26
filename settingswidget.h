#pragma once

#include <QWidget>
#include <QDebug>
#include <map>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QGridLayout;
QT_END_NAMESPACE

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    SettingsWidget();

signals:
    void widthChanged(int) const;
    void heightChanged(int) const;
    void speedChanged(int) const;
    void winScoreChanged(int) const;
    void penaltyChanged(int) const;

private slots:
    void onWidthChanged(const QString &) const;
    void onHeightChanged(const QString &) const;
    void onSpeedChanged(const QString &) const;
    void onWinScoreChanged(const QString &) const;
    void onPenaltyChanged(const QString &) const;

private:
    QLabel *m_widthLabel;
    QLineEdit *m_widthLineEdit;
    QLabel *m_heightLabel;
    QLineEdit *m_heightLineEdit;
    QLabel *m_speedLabel;
    QLineEdit *m_speedLineEdit;
    QLabel *m_winScoreLabel;
    QLineEdit *m_winScoreLineEdit;
    QLabel *m_penaltyLabel;
    QLineEdit *m_penaltyLineEdit;
    QGridLayout *m_mainLayout;

    void setupUi();
};

