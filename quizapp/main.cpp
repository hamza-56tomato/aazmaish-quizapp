#include "quizapp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    quizapp w;
    w.show();
    return a.exec();
}
