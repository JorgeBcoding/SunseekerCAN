#include "sunseekercan.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SunseekerCan w;
    w.show();
    
    return a.exec();
}
