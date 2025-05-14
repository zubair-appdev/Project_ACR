#include "health.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Health w;
    w.show();

    return a.exec();
}
