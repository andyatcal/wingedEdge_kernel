#include <QApplication>
#include <QPushButton>
#include "mymodel.h"
#include "MyGLWindow.h"
#include "MyGLWindow.cpp"
#include "MyWidget.h"
#include "MyWidget.cpp"

 int main(int argc, char *argv[])
 {
    QApplication app(argc, argv);
 	MyWidget mywidget;
 	mywidget.show();
    return app.exec();
 }