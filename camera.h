#ifndef CAMERA_H
#define CAMERA_H
#include <QGridLayout>
#include <QWebEngineView>
#include <QMainWindow>

class Camera
{
private:
    QWebEngineView *view;
    QMainWindow *mainWindow;

public:
    Camera(QGridLayout *cam);
    void move(int a);
};

#endif // CAMERA_H
