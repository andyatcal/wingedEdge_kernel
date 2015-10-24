#ifndef MY_WIDGET
#define MY_WIDGET

#include <QWIDGET>
#include "mymodel.h"

class MyWidget : public QWidget {
public:
	MyWidget();
private:
	MyGLWindow * myGLWindow;
	Model myModel;
};

#endif //MY_WIDGET
