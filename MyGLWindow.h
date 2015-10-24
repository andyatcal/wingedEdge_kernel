#ifndef MY_GL_WINDOW
#define MY_GL_WINDOW

#define PI 3.1415926535897932384626433832795028841971693993751058209
#define VERYSMALLVALUE 0.001

#include <QGLWIDGET>
#include <GLUT/glut.h>
#include <math.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <map>
#include <unordered_map>

#include "vertex.h"
#include "edge.h"
#include "face.h"

#include "mesh.h"
#include "merge.h"

#include "makeMesh.h"
#include "subdivision.h"
#include "viewport.h"
#include "offset.h"
#include "stl.h"
#include "mymodel.h"

class MyGLWindow : public QGLWidget {
public:
	void paintGL();
	void initializeGL();
private:
	MyModel * myModel;
};

#endif //MY_GL_WINDOW