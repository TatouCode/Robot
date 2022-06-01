#include "mainwindow.h"
#include "myrobot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    MyRobot robot(&w);
    //robot.bytesWritten();
    robot.doConnect();



    //w.show();
    return a.exec();
}
