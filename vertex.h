// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

// Forward declarations
class Edge;
class Mesh;

//////////////////////////////////////////////////////////////////////
// Vertex Class -- 3D Vertex for Catmull-Clark Subdivision.
//
class Vertex{
public:
    // The position of this vertex.
    vec3 position;
    // The normal of this Vertex.
    vec3 normal;
    // One out-going halfedge of this vertex.
    Edge * oneEdge;
    // An ID to track this Vertex.
    unsigned long ID;
    // A flag of on Mobius edge.
    bool onMobius;
    // A pointer to the vertex point after makeVertexPoints in subdivision.
    Vertex * vertexPoint;
    // A pointer to the positive offset of the vertex.
    Vertex * posOffset;
    // A pointer to the negative offset of the vertex.
    Vertex * negOffset;
    // Constructor.
    Vertex();
    // Contructor given initial location of vertex.
    // @param x, y, z as the coordinates of this vertex.
    Vertex(float x, float y, float z, unsigned long ID);
};

Vertex::Vertex(){
    position = vec3(0, 0, 0);
    normal = vec3(0, 0, 0);
    oneEdge = NULL;
    ID = 0;
}

Vertex::Vertex(float x, float y, float z, unsigned long ID) {
    position = vec3(x, y, z);
    normal = vec3(0, 0, 0);
    oneEdge = NULL;
    ID = ID;
}

#endif // __VERTEX_H__