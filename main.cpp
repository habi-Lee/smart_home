#include "widget.h"
#include "connnected.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connnected c;
    c.show();
    return a.exec();
}
