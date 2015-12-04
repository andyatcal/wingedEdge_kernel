// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __POLYLINE_H__
#define __POLYLINE_H__

#include <vector>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

// Forward declarations
class Vertex;
class Edge;


//////////////////////////////////////////////////////////////////////
// Polyline Class -- A Polyline is a list of vertices
class Polyline {
public:
    vector<Vertex*> vertices;
    bool isLoop;
    // Constructor
    Polyline();
    // Draw this polyline in OpenGL;
    void drawLine();
};

Polyline::Polyline()
{
    isLoop = false;
    vertices.clear();
}

void Polyline::drawLine()
{   
    if(!isLoop)
    {
        glBegin(GL_LINE_STRIP);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }
    vector<Vertex*>::iterator vIt;
    for(vIt = vertices.begin(); vIt < vertices.end(); vIt ++)
    {
        vec3 position = (*vIt) -> position;
        glVertex3f(position[0], position[1], position[2]);
    }
    glEnd();
}

#endif // __POLYLINE_H__