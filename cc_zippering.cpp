// Catmull-Clark Subdivision Project
// With sharp edge feature.
// With Mobius edge feature.
// Andy Wang, UC Berkeley July 1st 2015

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif
#ifdef __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif

#define PI 3.1415926535897932384626433832795028841971693993751058209
#define VERYSMALLVALUE 0.001
// Colors
GLfloat WHITE[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat RED[] = {0.8f, 0.0f, 0.0f, 1.0f};
GLfloat GREEN[] = {0.0f, 0.8f, 0.0f, 1.0f};
GLfloat BLUE[] = {0.0f, 0.0f, 0.8f, 1.0f};
GLfloat YELLOW[] = {.8f, .8f, 0.f, 1.f};
GLfloat PURPLE[] = {.8f, 0.f, .8f, 1.f};
GLfloat CYAN[] = {.0f, .8f, 0.8f, 1.f};
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
#include "polyline.h"
#include "mesh.h"
#include "merge.h"
#include "zipper.h"
#include "makeMesh.h"
#include "subdivision.h"
#include "viewport.h"
#include "offset.h"
#include "stl.h"

using namespace std;
using namespace glm;

//************************************************************
//          Global Variables
//************************************************************

// The viewport to diaplay graphics in OpenGL
Viewport viewport;
// USAGE MESSAGE
string USAGE = "USAGE: ./cc SUBDIVSION_LEVEL SIFFILE(OPTIONAL)";
//Initial state of wireframe mode
bool wireframe = false;
//Initial state of smooth shading
bool smoothShading = false;
//Initial state of GL_Light
bool lightOn = false; 
//Initial state of boarder selection
int boarderSelection = 0;
//Initial Rotation Angle
float angle = 0.0; 
//The distance of camera and orgin.
float cameraDistance = 10.0;
// The mesh to subdivide and display.
Mesh glMesh;
// Three meshes for offests display.
Mesh glPosMesh;
Mesh glNegMesh;
Mesh glSideMesh;
Mesh glOffMesh;

Mesh glMesh1;
Mesh glMesh2;
Mesh glMesh3;

// ArcBall feature.
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

// Viewer variables.
enum MODES { MODE_OBJECT, MODE_CAMERA, MODE_LIGHT, MODE_LAST } view_mode;

mat4 transforms[MODE_LAST];

vector<Vertex*> selectedVertices;

// For partial boarder selection
Vertex * firstBoarderSelectionPoint = NULL;
Vertex * secondBoarderSelectionPoint = NULL;
vector<Vertex*> firstHalfBoarder;
vector<Vertex*> vertToSelect;

// For zippering
Polyline * boarder1 = new Polyline;
Polyline * boarder2 = new Polyline;

//************************************************************
//          Let's build some Shapes!!
//************************************************************


// Initiate the mesh for OpenGL to render.
void init(int level);

void init(int level, string inputSIF);

void init(int level){
    //makeSquare(glMesh);
    //makePyramid(glMesh);
    //makeCube(glMesh, 1, 1, 1);
    //makeOpenCube(glMesh);
    makeRing(glMesh);
    //makeOctahedron(glMesh);
    //makeSharpOctahedron(glMesh);
    //makeNormalStrip(glMesh);
    //makeMobius(glMesh);
    //makeCircleSweep(glMesh);
    //glMesh.computeNormals();
    //glMesh = mergeTwoMeshes1();
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(level);
    glMesh.computeNormals();
    Offset offset(glMesh, 0.1);
    vector<Mesh> meshes;
    bool full = true;
    if(full) {
        offset.makeFullOffset();
        glOffMesh = offset.offsetMesh;
        Subdivision myOffCC(glOffMesh);
        glOffMesh = myOffCC.ccSubdivision(0);
        glOffMesh.computeNormals();
        meshes.push_back(glOffMesh);
    } else {
        offset.makeSeperateOffset();
        glPosMesh = offset.posOffsetMesh;
        glNegMesh = offset.negOffsetMesh;
        glSideMesh = offset.sideOffsetMesh;
        meshes.push_back(glPosMesh);
        meshes.push_back(glNegMesh);
        meshes.push_back(glSideMesh);
    }
    //STL stl;
    //stl.STLOutput(meshes, "debug/STL/Example.stl");
}

void init(int level, string inputSIF){
    //makeWithSIF(glMesh, inputSIF);
    makeWithQuadSIF(glMesh, inputSIF);
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(level);
    glMesh.computeNormals();
    
    Offset offset(glMesh, 0.0015);
    vector<Mesh> meshes;
    bool full = true;
    if(full) {
        offset.makeFullOffset();
        glOffMesh = offset.offsetMesh;
        Subdivision myOffCC(glOffMesh);
        glOffMesh = myOffCC.ccSubdivision(0);
        meshes.push_back(glOffMesh);
    } else {
        offset.makeSeperateOffset();
        glPosMesh = offset.posOffsetMesh;
        glNegMesh = offset.negOffsetMesh;
        glSideMesh = offset.sideOffsetMesh;
        meshes.push_back(glPosMesh);
        meshes.push_back(glNegMesh);
        meshes.push_back(glSideMesh);
    }
    
    STL stl;
    stl.STLOutput(meshes, "debug/STL/Example.stl");
    
}

void initMerge(int level, string inputSIF1, string inputSIF2, string inputSIF3){
    makeWithSIF(glMesh1, inputSIF1);
    makeWithSIF(glMesh2, inputSIF2);
    makeWithSIF(glMesh3, inputSIF3);
    Mesh glMesh12;
    glMesh12 = merge(glMesh1, glMesh2);
    glMesh = merge(glMesh12, glMesh3);
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(level);
    glMesh.computeNormals();
    
    Offset offset(glMesh, 0.0015);
    vector<Mesh> meshes;
    bool full = true;
    if(full) {
        offset.makeFullOffset();
        glOffMesh = offset.offsetMesh;
        Subdivision myOffCC(glOffMesh);
        glOffMesh = myOffCC.ccSubdivision(0);
        meshes.push_back(glOffMesh);
    } else {
        offset.makeSeperateOffset();
        glPosMesh = offset.posOffsetMesh;
        glNegMesh = offset.negOffsetMesh;
        glSideMesh = offset.sideOffsetMesh;
        meshes.push_back(glPosMesh);
        meshes.push_back(glNegMesh);
        meshes.push_back(glSideMesh);
    }
    
    STL stl;
    stl.STLOutput(meshes, "debug/STL/Example.stl");
    
}

// Zipper test
void initPolyline()
{
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;
    Vertex * v13 = new Vertex;
    Vertex * v14 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v11 -> ID = 11;
    v12 -> ID = 12;
    v13 -> ID = 13;
    v14 -> ID = 14;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0);
    v11 -> position = vec3(-1, 1, 0);
    v12 -> position = vec3(-0.5, 0, 0);
    v13 -> position = vec3(0, 0, 0);
    v14 -> position = vec3(1, 1, 0);
    v15 -> position = vec3(2.5, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v20 -> position = vec3(-3, -1, 0);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-0.5, -2, 0);
    v23 -> position = vec3(0, -2, 0);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 2.0);
    glMesh.computeNormals();
}

void initPolyline1()
{
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;
    Vertex * v13 = new Vertex;
    Vertex * v14 = new Vertex;
    v10 -> ID = 10;
    v11 -> ID = 11;
    v12 -> ID = 12;
    v13 -> ID = 13;
    v14 -> ID = 14;
    v10 -> position = vec3(-3, 1, 0);
    v11 -> position = vec3(-1, 1, 0);
    v12 -> position = vec3(-0.5, 2, 0);
    v13 -> position = vec3(0, 2, 0);
    v14 -> position = vec3(1, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v20 -> position = vec3(-3, -1, 0);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-0.5, -2, 0);
    v23 -> position = vec3(0, -2, 0);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 1.5);
    glMesh.computeNormals();
}

void initPolyline2()
{
    Vertex * v10 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0);
    v15 -> position = vec3(2.5, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v20 -> position = vec3(-3, -1, 0);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-0.5, -2, 0);
    v23 -> position = vec3(0, -2, 0);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 1.3);
    glMesh.computeNormals();
}

void initPolyline3()
{
    Vertex * v10 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0);
    v15 -> position = vec3(2.5, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v20 -> position = vec3(-3, -1, 0);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-0.5, -2, 0);
    v23 -> position = vec3(0, -2, 0);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 1.3);
    glMesh.computeNormals();
}

void initPolyline4()
{
    Vertex * v10 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0);
    v15 -> position = vec3(2.5, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v25 -> ID = 25;
    v20 -> position = vec3(-3, -1, 0);
    v25 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2);
    glMesh.computeNormals();
}

void initPolyline5()
{
    Vertex * v10 = new Vertex;
    v10 -> ID = 10;
    v10 -> position = vec3(-3, 1, 0);
    boarder1 -> vertices.push_back(v10);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v20 -> position = vec3(-3, -1, 0);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-0.5, -2, 0);
    v23 -> position = vec3(0, -2, 0);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2);
    glMesh.computeNormals();
}

void initPolyline6()
{
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;
    Vertex * v13 = new Vertex;
    Vertex * v14 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v11 -> ID = 11;
    v12 -> ID = 12;
    v13 -> ID = 13;
    v14 -> ID = 14;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0);
    v11 -> position = vec3(-1, 1, 0);
    v12 -> position = vec3(-0.5, 2, 0);
    v13 -> position = vec3(0, 2, 0);
    v14 -> position = vec3(1, 1, 0);
    v15 -> position = vec3(2.5, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v25 -> position = vec3(-3, -1, 0);
    v24 -> position = vec3(-1, -1, 0);
    v23 -> position = vec3(-0.5, -2, 0);
    v22 -> position = vec3(0, -2, 0);
    v21 -> position = vec3(1, -1, 0);
    v20 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 1.5);
    glMesh.computeNormals();
}

void initPolyline7()
{
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;
    Vertex * v13 = new Vertex;
    Vertex * v14 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v11 -> ID = 11;
    v12 -> ID = 12;
    v13 -> ID = 13;
    v14 -> ID = 14;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0);
    v11 -> position = vec3(-1, 1, 0);
    v12 -> position = vec3(-0.5, 2, 0);
    v13 -> position = vec3(0, 2, 0);
    v14 -> position = vec3(1, 1, 0);
    v15 -> position = vec3(2.5, 1, 0);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v25 -> position = vec3(-3, -1, 0);
    v24 -> position = vec3(-1, -1, 0);
    v23 -> position = vec3(-0.5, -2, 0);
    v22 -> position = vec3(0, -2, 0);
    v21 -> position = vec3(1, -1, 0);
    v20 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 1.5);
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(2);
    glMesh.computeNormals();
}

void initPolyline8()
{
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;
    Vertex * v13 = new Vertex;
    Vertex * v14 = new Vertex;
    Vertex * v15 = new Vertex;
    v10 -> ID = 10;
    v11 -> ID = 11;
    v12 -> ID = 12;
    v13 -> ID = 13;
    v14 -> ID = 14;
    v15 -> ID = 15;
    v10 -> position = vec3(-3, 1, 0.5);
    v11 -> position = vec3(-1, 1, 0.3);
    v12 -> position = vec3(-0.5, 2, 0.2);
    v13 -> position = vec3(0, 2, -0.3);
    v14 -> position = vec3(1, 1, -0.2);
    v15 -> position = vec3(2.5, 1, -0.5);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);
    boarder1 -> vertices.push_back(v15);

    Vertex * v20 = new Vertex;
    Vertex * v21 = new Vertex;
    Vertex * v22 = new Vertex;
    Vertex * v23 = new Vertex;
    Vertex * v24 = new Vertex;
    Vertex * v25 = new Vertex;
    v20 -> ID = 20;
    v21 -> ID = 21;
    v22 -> ID = 22;
    v23 -> ID = 23;
    v24 -> ID = 24;
    v25 -> ID = 25;
    v25 -> position = vec3(-3, -1, 0);
    v24 -> position = vec3(-1, -1, 0);
    v23 -> position = vec3(-0.5, -2, 0);
    v22 -> position = vec3(0, -2, 0);
    v21 -> position = vec3(1, -1, 0);
    v20 -> position = vec3(2.5, -1, 0);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    Zipper zipper;
    glMesh = zipper.zip(boarder1, boarder2, 1.5);
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(2);
    glMesh.computeNormals();
}

//************************************************************
//          Arcball Functions
//************************************************************

/**
 * Get a normalized vector from the center of the virtual ball O to a
 * point P on the virtual ball surface, such that P is aligned on
 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 * sphere, return the nearest point on the virtual ball surface.
 */
vec3 get_arcball_vector(int x, int y) {
    vec3 P = vec3(1.0 * x / viewport.width * 2 - 1.0,
      1.0 * y / viewport.height * 2 - 1.0, 0);
    P.y = - P.y;
    float OP_squared = P.x * P.x + P.y * P.y;
    if (OP_squared <= 1 * 1) {
        P.z = sqrt(1 * 1 - OP_squared);
    } else {
        P = normalize(P);  // nearest point
    }
    return P;
}

//************************************************************
//          OpenGL Display Functions
//************************************************************
void initRendering();

void render(void);

void reshape(int w, int h);

void keyboard(unsigned char c, int x, int y);

void keySpecial(int key, int x, int y);

void onMouse(int button, int state, int x, int y);

void onMotion(int x, int y);

void initRendering(){

    // Two sided pr ones side;
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_ambient0[] = { 0.8, 0.8, 0.8, 10.0 };
    GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 10.0 };
    GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 10.0 };
    GLfloat light_position0[] = { 10, 10, 10, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
/*
    glEnable(GL_LIGHT1);

    GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position1[] = { 0, 1, -0.5, 0.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glEnable(GL_LIGHT2);

    GLfloat light_ambient2[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position2[] = { 0.877, -0.5, -0.5, 0.0 };

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

    glEnable(GL_LIGHT3);

    GLfloat light_ambient3[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position3[] = { -0.877, -0.5, -0.5, 0.0 };

    glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

    //glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    //glMaterialfv(GL_BACK, GL_DIFFUSE, purple);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    //GLfloat shininess[] = {50};
    //glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
*/
    transforms[MODE_CAMERA] = lookAt(
        vec3(0.0,  0.0, 10.0),   // eye
        vec3(0.0,  0.0, 0.0),   // direction
        vec3(0.0,  1.0, 0.0));  // up
}

void drawVerticesAsSphere() {
    vector<Vertex*> vertList = glMesh.vertList;
    vector<Vertex*>::iterator vIt;
    glPointSize(10);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    int counter = 0;
    for(vIt = vertList.begin(); vIt < vertList.end(); vIt++) {
        glLoadName(counter);
        glPushMatrix();
            glTranslated((*vIt) -> position[0],(*vIt) -> position[1],(*vIt) -> position[2]);
            if((*vIt) -> selected) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, GREEN);
            }
            glutSolidSphere(0.0003,50,50);
        glPopMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
        counter += 1;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CYAN);
}

void drawVertices() {
    vector<Vertex*> vertList = glMesh.vertList;
    vector<Vertex*>::iterator vIt;
    glPointSize(10);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    int counter = 0;
    for(vIt = vertList.begin(); vIt < vertList.end(); vIt++) {
        if((*vIt) -> selected) {
            glBegin(GL_POINTS);
            vec3 position = (*vIt) -> position;
            glNormal3f(position[0], position[1], position[2]);
            glVertex3f(position[0], position[1], position[2]);
            //
            glEnd();
        }
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CYAN);
}

void render(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

    glMultMatrixf(&glMesh.object2world[0][0]);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, YELLOW);
    drawVertices();
    //glLoadName(INT_MAX);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    boarder1 -> drawLine();
    boarder2 -> drawLine();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CYAN);
    glMesh.drawMesh();
    //drawVertices();

/*
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, GREEN);
    glPosMesh.drawMesh();
   
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    glNegMesh.drawMesh();

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CYAN);
    glSideMesh.drawMesh();

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BLUE);
    glOffMesh.drawMesh();
*/
    glutSwapBuffers();

}

void reshape(int w, int h) {
    if (h == 0) {h = 1;} // prevent a division by zero when setting aspect ratio
    viewport.width = w;
    viewport.height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluPerspective(45, (float) w / h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void onIdle() {
    if(last_mx != cur_mx || last_my != cur_my) {
        glm::vec3 va = get_arcball_vector(last_mx, last_my);
        glm::vec3 vb = get_arcball_vector( cur_mx,  cur_my);
        float angle = acos(glm::min(1.0f, dot(va, vb)));
        glm::vec3 axis_in_camera_coord = cross(va, vb);
        glm::mat3 camera2object = inverse(mat3(transforms[MODE_CAMERA]) * mat3(glMesh.object2world));
        glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        glMesh.object2world = rotate(glMesh.object2world, (float) 1.0 * angle, axis_in_object_coord);
        last_mx = cur_mx;
        last_my = cur_my;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
    if (key == 's') {
        if (smoothShading) {
            glShadeModel(GL_FLAT);
            smoothShading = false;
        } else {
            glShadeModel(GL_SMOOTH);
            smoothShading = true;
        }
    }
    if (key == 'w') {
        if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            wireframe = false;
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            wireframe = true;
        }
    }
    if (key == 'l') {
        if (lightOn) {
            glDisable(GL_LIGHTING);
            lightOn = false;
        } else {
            glEnable(GL_LIGHTING);
            lightOn = true;
        }
    }
    if (key == 'i') {
        if(cameraDistance > 0.1) {
            cameraDistance *= 0.9;
        }
    }
    if (key == 'o') {
        if(cameraDistance < 200) {
            cameraDistance *= 1.1;
        }
    }
    if (key == 'b') {
        if (boarderSelection == 2) {
            cout<<"Boarder Selection Switch OFF."<<endl;
            boarderSelection = 0;
        } else if (boarderSelection == 0) {
            cout<<"Whole Boarder Selection Switch ON."<<endl;
            boarderSelection = 1;
        } else {
            cout<<"Partial Boarder Selection Switch ON. (Whole Boarder Selection OFF.)"<<endl;
            boarderSelection = 2;
        }
    }
    glutPostRedisplay();
}

void keySpecial(int key, int x, int y) {
    bool shiftDown = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
    if (key == 27) {
        exit(0);
    }
    glutPostRedisplay();
}

void list_hits(GLint hits, GLuint *names)
{
    int i;
 
      /*
         For each hit in the buffer are allocated 4 bytes:
         1. Number of hits selected (always one,
         beacuse when we draw each object
         we use glLoadName, so we replace the
         prevous name in the stack)
         2. Min Z
         3. Max Z
         4. Name of the hit (glLoadName)
         */
     
    printf("%d hits:\n", hits);
     
    for (i = 0; i < hits; i++)
        printf( "Number: %d\n"
                "Min Z: %d\n"
                "Max Z: %d\n"
                "Name on stack: %d\n",
                (GLubyte)names[i * 4],
                (GLubyte)names[i * 4 + 1],
                (GLubyte)names[i * 4 + 2],
                (GLubyte)names[i * 4 + 3]
            );
        printf("\n");
}

void selectThisFace(GLint hits, GLuint *names)
{
    if(hits > 0) {
        int minimumDepth = INT_MAX;
        int minimumDepthIndex = INT_MAX;
        for (int i = 0; i < hits; i++) {
            int currentDepth = (GLubyte)names[i * 4 + 1];
            if(currentDepth < minimumDepth) {
                minimumDepth = currentDepth;
                minimumDepthIndex = i;
            }
        }
        int selectedID = names[minimumDepthIndex * 4 + 3];
        Face * workFace = glMesh.faceList[selectedID];
        if(!workFace->selected) {
            workFace->selected = true;
        } else {
            workFace->selected = false;
        }
    }
}

void selectPointFromThisFace(GLint hits, GLuint *names, GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        int minimumDepth = INT_MAX;
        int minimumDepthIndex = INT_MAX;
        for (int i = 0; i < hits; i++) {
            int currentDepth = (GLubyte)names[i * 4 + 1];
            if(currentDepth < minimumDepth) {
                minimumDepth = currentDepth;
                minimumDepthIndex = i;
            }
        }
        int selectedID = names[minimumDepthIndex * 4 + 3];
        Face * workFace = glMesh.faceList[selectedID];
        Edge * firstEdge = workFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Vertex * tempv;
        Vertex * selectedVertex;
        float minDistance = 50000.0; // A very large value ...
        do {
            if(workFace == currEdge -> fa) {
                tempv = currEdge -> vb;
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    tempv = currEdge -> vb;
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    tempv = currEdge -> va;
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            float newDistance = distance(tempv -> position, vec3(posX, posY, posZ));
            if(newDistance < minDistance) {
                minDistance = newDistance;
                selectedVertex = tempv;
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        if(selectedVertex -> selected) {
            selectedVertex -> selected = false;
        } else {
            selectedVertex -> selected = true;
        }
    }
}

void selectWholeBoarderPointsFromThisFace(GLint hits, GLuint *names, GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        int minimumDepth = INT_MAX;
        int minimumDepthIndex = INT_MAX;
        for (int i = 0; i < hits; i++) {
            int currentDepth = (GLubyte)names[i * 4 + 1];
            if(currentDepth < minimumDepth) {
                minimumDepth = currentDepth;
                minimumDepthIndex = i;
            }
        }
        int selectedID = names[minimumDepthIndex * 4 + 3];
        Face * workFace = glMesh.faceList[selectedID];
        Edge * firstEdge = workFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Vertex * selectedVertex = NULL;
        Edge * startingEdge;
        Vertex * tempva;
        Vertex * tempvb;
        float minDistance = 50000.0; // A very large value ...
        do {
            if(workFace == currEdge -> fa) {
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(currEdge -> fb == NULL) {
                tempva = currEdge -> va;
                tempvb = currEdge -> vb;
                float newDistanceA = distance(tempva -> position, vec3(posX, posY, posZ));
                float newDistanceB = distance(tempvb -> position, vec3(posX, posY, posZ));
                if(minDistance > newDistanceA) {
                    minDistance = newDistanceA;
                    selectedVertex = tempva;
                    startingEdge = currEdge;
                }
                if(minDistance > newDistanceB) {
                    minDistance = newDistanceB;
                    selectedVertex = tempvb;
                    startingEdge = currEdge;
                }
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        if(selectedVertex != NULL) {
            if(selectedVertex -> selected) {
                Vertex * nextVert = selectedVertex;
                Edge * nextBoarderEdge = startingEdge;
                do {
                    nextVert -> selected = false;
                    //cout<<nextVert -> ID;
                    if(nextVert == nextBoarderEdge -> va) {
                        nextBoarderEdge = nextBoarderEdge -> nextVaFb;
                    } else {
                        nextBoarderEdge = nextBoarderEdge -> nextVbFb;
                    }
                    if(nextBoarderEdge -> va == nextVert) {
                        nextVert = nextBoarderEdge -> vb;
                    } else {
                        nextVert = nextBoarderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
            } else {
                Vertex * nextVert = selectedVertex;
                Edge * nextBoarderEdge = startingEdge;
                do {
                    nextVert -> selected = true;
                    //cout<<nextVert -> ID;
                    if(nextVert == nextBoarderEdge -> va) {
                        nextBoarderEdge = nextBoarderEdge -> nextVaFb;
                    } else {
                        nextBoarderEdge = nextBoarderEdge -> nextVbFb;
                    }
                    if(nextBoarderEdge -> va == nextVert) {
                        nextVert = nextBoarderEdge -> vb;
                    } else {
                        nextVert = nextBoarderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
            }
        }
    }
}

void selectPartialBoarderPoints(GLint hits, GLuint *names, GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        int minimumDepth = INT_MAX;
        int minimumDepthIndex = INT_MAX;
        for (int i = 0; i < hits; i++) {
            int currentDepth = (GLubyte)names[i * 4 + 1];
            if(currentDepth < minimumDepth) {
                minimumDepth = currentDepth;
                minimumDepthIndex = i;
            }
        }
        int selectedID = names[minimumDepthIndex * 4 + 3];
        Face * workFace = glMesh.faceList[selectedID];
        Edge * firstEdge = workFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        Vertex * selectedVertex = NULL;
        Edge * startingEdge;
        Vertex * tempva;
        Vertex * tempvb;
        float minDistance = 50000.0; // A very large value ...
        do {
            if(workFace == currEdge -> fa) {
                nextEdge = currEdge -> nextVbFa;
            } else {
                if(currEdge -> mobius) {
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            if(currEdge -> fb == NULL) {
                tempva = currEdge -> va;
                tempvb = currEdge -> vb;
                float newDistanceA = distance(tempva -> position, vec3(posX, posY, posZ));
                float newDistanceB = distance(tempvb -> position, vec3(posX, posY, posZ));
                if(minDistance > newDistanceA) {
                    minDistance = newDistanceA;
                    selectedVertex = tempva;
                    startingEdge = currEdge;
                }
                if(minDistance > newDistanceB) {
                    minDistance = newDistanceB;
                    selectedVertex = tempvb;
                    startingEdge = currEdge;
                }
            }
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        if(selectedVertex) {
            if (!firstBoarderSelectionPoint) {
                firstBoarderSelectionPoint = selectedVertex;
                cout<<"Selecting the first Point on boarder."<<endl;
                selectedVertex -> selected = true;
                Vertex * nextVert = selectedVertex;
                Edge * nextBoarderEdge = startingEdge;
                do {
                    firstHalfBoarder.push_back(nextVert);
                    //cout<<nextVert -> ID<<endl;;
                    if(nextVert == nextBoarderEdge -> va) {
                        nextBoarderEdge = nextBoarderEdge -> nextVaFb;
                    } else {
                        nextBoarderEdge = nextBoarderEdge -> nextVbFb;
                    }
                    if(nextBoarderEdge -> va == nextVert) {
                        nextVert = nextBoarderEdge -> vb;
                    } else {
                        nextVert = nextBoarderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
                //cout<<firstHalfBoarder.size()<<endl;
            } else if (!secondBoarderSelectionPoint) {
                //We need to test if this second point in the boarder of the first selecte point.
                vector<Vertex*>::iterator vIt;
                for(vIt = firstHalfBoarder.begin(); vIt < firstHalfBoarder.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBoarderSelectionPoint) {
                            cout<<"Unselecting the first point on boarder."<<endl;
                            firstBoarderSelectionPoint -> selected = false;
                            firstBoarderSelectionPoint = NULL;
                            firstHalfBoarder.clear();
                            //(*vIt) -> selected = false;
                        }
                        break;
                    }
                }
                if(!firstHalfBoarder.empty()) {
                    if(vIt == firstHalfBoarder.end()) {
                        cout<<"Your choice of the two points is not on the same boarder. Selection ABORT."<<endl;
                        firstBoarderSelectionPoint -> selected = false;
                        firstBoarderSelectionPoint = NULL;
                        firstHalfBoarder.clear();
                    } else {
                        cout<<"Selecting the second Point on boarder."<<endl;
                        secondBoarderSelectionPoint = selectedVertex;
                        selectedVertex -> selected = true;
                    }
                }
            } else if(!vertToSelect.empty()) {
                cout<<"Unselecting all points on partial boarder."<<endl;
                vector<Vertex*>::iterator vIt;
                for(vIt = vertToSelect.begin(); vIt < vertToSelect.end(); vIt++) {
                    (*vIt) -> selected = false;
                }
                firstBoarderSelectionPoint -> selected = false;
                secondBoarderSelectionPoint -> selected = false;
                firstBoarderSelectionPoint = NULL;
                secondBoarderSelectionPoint = NULL;
                firstHalfBoarder.clear();
                vertToSelect.clear();
            } else {
                vector<Vertex*>::iterator vIt;
                bool hasSeenSecondPoint = false;
                for(vIt = firstHalfBoarder.begin(); vIt < firstHalfBoarder.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBoarderSelectionPoint || (*vIt) == secondBoarderSelectionPoint) {
                            cout<<"Unselecting points on boarder."<<endl;
                            firstBoarderSelectionPoint -> selected = false;
                            secondBoarderSelectionPoint -> selected = false;
                            firstBoarderSelectionPoint = NULL;
                            secondBoarderSelectionPoint = NULL;
                            firstHalfBoarder.clear();
                        }
                        break;
                    } else if((*vIt) == secondBoarderSelectionPoint) {
                        hasSeenSecondPoint = true;
                    }
                }
                if(!firstHalfBoarder.empty()) {
                    if(vIt == firstHalfBoarder.end()) {
                        cout<<"Your choice of the third points is not on the same boarder. Selection ABORT."<<endl;
                        firstBoarderSelectionPoint -> selected = false;
                        secondBoarderSelectionPoint -> selected = false;
                        firstBoarderSelectionPoint = NULL;
                        secondBoarderSelectionPoint = NULL;
                        firstHalfBoarder.clear();
                    } else {
                        cout<<"Selecting the half boarder on this side."<<endl;
                        vector<Vertex*>::iterator vSelIt;
                        if(!hasSeenSecondPoint) {
                            for(vSelIt = firstHalfBoarder.begin(); vSelIt < firstHalfBoarder.end(); vSelIt++) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBoarderSelectionPoint) {
                                    break;
                                }
                            }
                        } else {
                            vertToSelect.push_back(firstBoarderSelectionPoint);
                            for(vSelIt = firstHalfBoarder.end() - 1; vSelIt >= firstHalfBoarder.begin(); vSelIt--) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBoarderSelectionPoint) {
                                    break;
                                }
                            }
                        }
                        for(vSelIt = vertToSelect.begin(); vSelIt < vertToSelect.end(); vSelIt++) {
                            (*vSelIt) -> selected = true;
                        }
                    }
                }
            }
        }
    }
}

void selectThisPoint(GLint hits, GLuint *names)
{
    int minimumDepth = INT_MAX;
    int minimumDepthIndex = INT_MAX;
    for (int i = 0; i < hits; i++) {
        int currentDepth = (GLubyte)names[i * 4 + 1];
        if(currentDepth < minimumDepth) {
            minimumDepth = currentDepth;
            minimumDepthIndex = i;
        }
    }
    int selectedID = names[minimumDepthIndex * 4 + 3];
    if(selectedID < glMesh.vertList.size()) {
        Vertex * workVert = glMesh.vertList[selectedID];
        if(!workVert->selected) {
            workVert->selected = true;
        } else {
            workVert->selected = false;
        }
    }
}

void select(int x, int y) {
    GLuint buff[64] = {0};
    GLint hits, view[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    glSelectBuffer(64, buff);
    glGetIntegerv(GL_VIEWPORT, view);
    // Find the 3D points of the current clicked point
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev(GL_PROJECTION_MATRIX, projection );
    winX = (float)x;
    winY = (float)view[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject( winX, winY, winZ, modelview, projection, view, &posX, &posY, &posZ);
    //cout<<"X: "<<posX<<" Y: "<<posY<<" Z: "<<posZ<<endl;
    // Find the face selected.
    glRenderMode(GL_SELECT);
    //glClearColor(0, 0, 0, 1);
    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(x, view[3] - y, 1.0, 1.0, view);
    gluPerspective(45, (float) viewport.width / viewport.height, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
    render();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    hits = glRenderMode(GL_RENDER);
    //list_hits(hits, buff);
    //selectThisFace(hits, buff);
    if(boarderSelection == 0){
        selectPointFromThisFace(hits, buff, posX, posY, posZ);
    } else if( boarderSelection == 1) {
        selectWholeBoarderPointsFromThisFace(hits, buff, posX, posY, posZ);
    } else {
        selectPartialBoarderPoints(hits, buff, posX, posY, posZ);
    }
    //selectPoint(hits, buff);
    glMatrixMode(GL_MODELVIEW);
}

void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        arcball_on = true;
        last_mx = cur_mx = x;
        last_my = cur_my = y;
    } else {
        arcball_on = false;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        //printf("Mouse button %d pressed at %d %d\n", button, x, y);
        select(x, y);
    }
}

void onMotion(int x, int y){
  if (arcball_on) {  // if left button is pressed
    cur_mx = x;
    cur_my = y;
  }
}

//************************************************************
//          Main Function
//************************************************************

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    viewport.width = 640;
    viewport.height = 480;

    //Process the command line arguments
    int level;
    string inputSIF;
    string inputSIF1;
    string inputSIF2;
    string inputSIF3;
    /*
    if(argc == 1 || argc > 5){
        cout<<"USAGE: ./NAME_OF_PROGRAM LEVEL_OF_SUBDIVISION INPUT_SIF_FILE.";
        exit(1);
    } else if(argc == 2){
        level = stoi(argv[1]);
        init(level);
    } else if(argc == 3){
        level = stoi(argv[1]);
        inputSIF = argv[2];
        init(level, inputSIF);
    } else if(argc == 5){
        level = stoi(argv[1]);
        inputSIF1 = argv[2];
        inputSIF2 = argv[3];
        inputSIF3 = argv[4];
        initMerge(level, inputSIF1, inputSIF2, inputSIF3);
    }
    */
    int testcase = stoi(argv[1]);
    if(testcase == 0) {
        initPolyline();
    } else if (testcase == 1) {
        initPolyline1();
    } else if (testcase == 2) {
        initPolyline2();
    } else if (testcase == 3) {
        initPolyline3();
    } else if (testcase == 4) {
        initPolyline4();
    } else if (testcase == 5) {
        initPolyline5();
    } else if (testcase == 6) {
        initPolyline6();
    } else if (testcase == 7) {
        initPolyline7();
    } else if (testcase == 8) {
        initPolyline8();
    }
    
    glutInitWindowSize(viewport.width, viewport.height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(0.8, 0.8, 0.8, 1.0f);
    initRendering();
    glutDisplayFunc(render);
    // General UI functions
    glutReshapeFunc(reshape);
    glutIdleFunc(onIdle); 
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keySpecial);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);

    glutMainLoop();

}

