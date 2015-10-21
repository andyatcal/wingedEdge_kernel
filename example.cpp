
#include <QPushButton>
#include "MyGLWindow.h"

 int main(int argc, char *argv[])
 {
    QApplication app(argc, argv);
    MyGLWindow myWindow;
    myWindow.show();
    return app.exec();
 }