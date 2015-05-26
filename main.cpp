#include <QtWidgets>

#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget widget;

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    widget.show();
    return a.exec();
}
