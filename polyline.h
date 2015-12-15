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
    // Return the skewness of points in this polyline.
    vector<float> skewness();
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

vector<float> Polyline::skewness()
{
    if(vertices.size() < 2) {
        cout<<"ERROR: Can not find the skewness of a polyline with one vertex."<<endl;
        exit(0);
    }
    vector<float> distances;
    distances.push_back(0);
    vector<Vertex*>::iterator vIt;
    float sum = 0;
    for(vIt = vertices.begin(); vIt < vertices.end() - 1; vIt++) {
        float currDistance = distance((*(vIt + 1)) -> position, (*vIt) -> position);
        sum += currDistance;
        distances.push_back(sum);
    }
    if(isLoop) {
        sum += distance(vertices[0] -> position, (*vIt) -> position);
    }
    vector<float>::iterator dIt;
    for(dIt = distances.begin(); dIt < distances.end(); dIt++) {
        (*dIt) /= sum;
        cout<<(*dIt)<<endl;
    }
    return distances;
}

#endif // __POLYLINE_H__