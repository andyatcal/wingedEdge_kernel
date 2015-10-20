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

#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
//Initial Rotation Angle
float angle = 0.0; 
//The distance of camera and orgin.
float cameraDistance = 4.0;
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

// Colors
GLfloat WHITE[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat RED[] = {0.8f, 0.0f, 0.0f, 1.0f};
GLfloat GREEN[] = {0.0f, 0.8f, 0.0f, 1.0f};
GLfloat BLUE[] = {0.0f, 0.0f, 0.8f, 1.0f};
GLfloat YELLOW[] = {.8f, .8f, 0.f, 1.f};
GLfloat PURPLE[] = {.8f, 0.f, .8f, 1.f};
GLfloat CYAN[] = {.0f, .8f, 0.8f, 1.f};

//************************************************************
//          Let's build some Shapes!!
//************************************************************


// Initiate the mesh for OpenGL to render.
void init(int level);

void init(int level, string inputSIF);

void init(int level){
    //makeSquare(glMesh);
    //makePyramid(glMesh);
    makeCube(glMesh, 1, 0.7, 0.85);
    //makeOpenCube(glMesh);
    //makeRing(glMesh);
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
    STL stl;
    stl.STLOutput(meshes, "debug/STL/Example.stl");
}

void init(int level, string inputSIF){
    makeWithSIF(glMesh, inputSIF);
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
    GLfloat light_position0[] = { 1, 1, 1, 0.0 };

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


void render(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

    glMultMatrixf(&glMesh.object2world[0][0]);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    glMesh.drawMesh();
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
        glMesh.object2world = rotate(glMesh.object2world, angle, axis_in_object_coord);
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
    glutPostRedisplay();
}

void keySpecial(int key, int x, int y) {
    bool shiftDown = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
    if (key == 27) {
        exit(0);
    }
    glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        arcball_on = true;
        last_mx = cur_mx = x;
        last_my = cur_my = y;
    } else {
        arcball_on = false;
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

