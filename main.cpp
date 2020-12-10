#include "ImageProcessUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageProcessUI w;
    w.show();
    return a.exec();
}
