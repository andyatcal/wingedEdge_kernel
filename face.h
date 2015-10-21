// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __FACE_H__
#define __FACE_H__

#include "glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

// Forward declarations
class Vertex;

//////////////////////////////////////////////////////////////////////
// Face Class -- Polygon Face for Catmull-Clark Subdivision.
//
class Face{
public:
    // The surface normal of this face.
    vec3 normal;
    // A pointer to one edge that constructs this face.
    Edge * oneEdge;
    // A pointer to the face point after makeFacePoints in subdivision.
    Vertex * facePoint;
    // Constructor.
    Face();
};

Face::Face(){
    oneEdge = NULL;
    facePoint = NULL;
}

#endif // __FACE_H__