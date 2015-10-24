#include <QGLWIDGET>
#include <GLUT/glut.h>
#include "MyGLWindow.h"

MyGLWindow(MyModel * model) {
	myModel = model;
}

void MyGLWindow::paintGL()
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void MyGLWindow::initializeGL()
{
	glInit();
}