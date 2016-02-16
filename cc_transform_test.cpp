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
#include "glm/gtc/constants.hpp"
#include "glm/gtx/string_cast.hpp"
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
#include "subdivision.h"
#include "makePolyline.h"
#include "transformation.h"
#include "viewport.h"
#include "makeMesh.h"
#include "group.h"
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
//The distance of camera and orgin.
float cameraDistance = 5.0;

//************************************************************
//          Global Variables for Pipeline
//************************************************************

// The master mesh
Mesh master_mesh;

// The mesh after subdivision
Mesh master_sub_mesh;

// The temporary mesh
Mesh temp_mesh;

// A copy of master_mesh
Mesh copy_mesh;

// Mode: Adding (0) or Zipping (1). Default as adding.
int addingMode = 0;

// Vertices selected. Can be used to create Face or Polyline.
vector<Vertex*> selectedVertices;

// For Zipping Mode.
Polyline * border1 = NULL;
Polyline * border2 = NULL;

// Boader selection mode. Single point (0),
// Whole border (1), Partial border (2).
int borderSelection = 0;

// For partial border selection
Vertex * firstBorderSelectionPoint = NULL;
Vertex * secondBorderSelectionPoint = NULL;
vector<Vertex*> allBorderPoints;
vector<Vertex*> vertToSelect;

//************************************************************
//          Global Variables for Arcball
//************************************************************
// ArcBall feature.
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

// Viewer variables.
enum MODES { MODE_OBJECT, MODE_CAMERA, MODE_LIGHT, MODE_LAST } view_mode;
mat4 transforms[MODE_LAST];


//************************************************************
//          Process A. Create Master Mesh
//************************************************************

// Manually Create. (From functions of MakeMesh class)
void init(int level){
    //makeMobius(master_mesh);
    //makeSquare(master_mesh);
    //master_mesh = mergeTwoMeshes2();
    //master_mesh = mergeTwoMeshes3();
    master_mesh = mergeTwoMeshes4();
    //master_mesh = mergeTwoMeshes5();
    //master_mesh = mirrorTest();
    master_mesh.computeNormals();
}

void init(int level, string inputSIF){
    makeWithSIF(master_mesh, inputSIF);
    //makeWithQuadSIF(master_mesh, inputSIF);
    master_mesh.computeNormals();
}


//************************************************************
//          Process B. SELECTION
//************************************************************

// Single point selection.
void selectSinglePoint(GLint hits, GLuint *names,
 GLdouble posX, GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = master_mesh.faceList[currentID];
            Edge * firstEdge = workFace -> oneEdge;
            Edge * currEdge = firstEdge;
            Edge * nextEdge;
            Vertex * tempv;
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
                float new_distance = distance(tempv -> position, hit_position);
                if(new_distance < min_distance) {
                    min_distance = new_distance;
                    selectedVertex = tempv;
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        if(selectedVertex -> selected) {
            selectedVertex -> selected = false;
            vector<Vertex*>::iterator vIt;
            for(vIt = selectedVertices.begin();
             vIt < selectedVertices.end(); vIt ++) {
                if((*vIt) == selectedVertex) {
                    break;
                }
            }
            selectedVertices.erase(vIt);
            cout<<"Unselected Vertex: v"<<selectedVertex -> ID<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        } else {
            selectedVertex -> selected = true;
            selectedVertices.push_back(selectedVertex);
            cout<<"Selected Vertex: v"<<selectedVertex -> ID<<endl;
            cout<<"You have "<<selectedVertices.size()
            <<" vertices selected."<<endl;
        }
    }
}

// Boarder Points selection
void selectWholeBorder(GLint hits, GLuint *names, GLdouble posX,
 GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = master_mesh.faceList[currentID];
            Edge * firstEdge = workFace -> oneEdge;
            Edge * currEdge = firstEdge;
            Edge * nextEdge;
            Vertex * tempv;
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
                float new_distance = distance(tempv -> position, hit_position);
                if(new_distance < min_distance) {
                    min_distance = new_distance;
                    selectedVertex = tempv;
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        // Test is this point is on border. If yes, find the startingEdge.
        if(selectedVertex != NULL) {
            Edge * firstEdge = selectedVertex -> oneEdge;
            Edge * currEdge = firstEdge;
            Face * currFace = currEdge -> fb;
            Edge * nextEdge;
            do {
                if(currFace == NULL) {
                    break;
                } else {
                    nextEdge = currEdge -> nextEdge(selectedVertex, currFace);
                    currFace = nextEdge -> theOtherFace(currFace);
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
            if(currFace != NULL) {
                cout<<"Your selected vertex is not on the border."
                <<" Current Selection Cancelled.\n";
                return;
            }
            if(selectedVertex -> selected) {
                cout<<"Unselecting all points on this border.\n";
                selectedVertices.clear();
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = currEdge;
                do {
                    nextVert -> selected = false;
                    //cout<<nextVert -> ID;
                    if(nextVert == nextBorderEdge -> va) {
                        nextBorderEdge = nextBorderEdge -> nextVaFb;
                    } else {
                        nextBorderEdge = nextBorderEdge -> nextVbFb;
                    }
                    if(nextBorderEdge -> va == nextVert) {
                        nextVert = nextBorderEdge -> vb;
                    } else {
                        nextVert = nextBorderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
            } else {
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = currEdge;
                do {
                    selectedVertices.push_back(nextVert);
                    nextVert -> selected = true;
                    //cout<<nextVert -> ID;
                    if(nextVert == nextBorderEdge -> va) {
                        nextBorderEdge = nextBorderEdge -> nextVaFb;
                    } else {
                        nextBorderEdge = nextBorderEdge -> nextVbFb;
                    }
                    if(nextBorderEdge -> va == nextVert) {
                        nextVert = nextBorderEdge -> vb;
                    } else {
                        nextVert = nextBorderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
                cout<<"You have selected "<<selectedVertices.size()
                <<" points on this border.\n";
            }
        }
    }
}

void selectPartialBorder(GLint hits, GLuint *names, GLdouble posX,
 GLdouble posY, GLdouble posZ)
{
    if(hits > 0) {
        vec3 hit_position = vec3(posX, posY, posZ);
        float min_distance = 500000.0;
        Vertex * selectedVertex;
        for (int i = 0; i < hits; i++) {
            int currentID = names[i * 4 + 3];
            Face * workFace = master_mesh.faceList[currentID];
            Edge * firstEdge = workFace -> oneEdge;
            Edge * currEdge = firstEdge;
            Edge * nextEdge;
            Vertex * tempv;
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
                float new_distance = distance(tempv -> position, hit_position);
                if(new_distance < min_distance) {
                    min_distance = new_distance;
                    selectedVertex = tempv;
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
        }
        // Test is this point is on border. If yes, find the startingEdge.
        if(selectedVertex != NULL) {
            Edge * firstEdge = selectedVertex -> oneEdge;
            Edge * currEdge = firstEdge;
            Face * currFace = currEdge -> fb;
            Edge * nextEdge;
            do {
                if(currFace == NULL) {
                    break;
                } else {
                    nextEdge = currEdge -> nextEdge(selectedVertex, currFace);
                    currFace = nextEdge -> theOtherFace(currFace);
                }
                currEdge = nextEdge;
            } while (currEdge != firstEdge);
            if(currFace != NULL) {
                cout<<"Your selected vertex is not on the border."
                <<" Current Selection Cancelled.\n";
                return;
            }
            if (!firstBorderSelectionPoint) {
                firstBorderSelectionPoint = selectedVertex;
                cout<<"Selecting the first Point on border."<<endl;
                selectedVertex -> selected = true;
                selectedVertices.push_back(selectedVertex);
                Vertex * nextVert = selectedVertex;
                Edge * nextBorderEdge = currEdge;
                do {
                    allBorderPoints.push_back(nextVert);
                    //cout<<nextVert -> ID<<endl;;
                    if(nextVert == nextBorderEdge -> va) {
                        nextBorderEdge = nextBorderEdge -> nextVaFb;
                    } else {
                        nextBorderEdge = nextBorderEdge -> nextVbFb;
                    }
                    if(nextBorderEdge -> va == nextVert) {
                        nextVert = nextBorderEdge -> vb;
                    } else {
                        nextVert = nextBorderEdge -> va;
                    }
                } while (nextVert != selectedVertex);
            } else if (!secondBorderSelectionPoint) {
                // We need to test if this second point in the border
                // of the first selecte point.
                vector<Vertex*>::iterator vIt;
                for(vIt = allBorderPoints.begin();
                 vIt < allBorderPoints.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBorderSelectionPoint) {
                            cout<<"Unselecting the first point on border."<<endl;
                            firstBorderSelectionPoint -> selected = false;
                            firstBorderSelectionPoint = NULL;
                            allBorderPoints.clear();
                            selectedVertices.clear();
                            //(*vIt) -> selected = false;
                        }
                        break;
                    }
                }
                if(!allBorderPoints.empty()) {
                    if(vIt == allBorderPoints.end()) {
                        cout<<"Your choice of the two points are not on the"
                        <<" same border. Selection ABORT."<<endl;
                        firstBorderSelectionPoint -> selected = false;
                        firstBorderSelectionPoint = NULL;
                        allBorderPoints.clear();
                        selectedVertices.clear();
                    } else {
                        cout<<"Selecting the second point on border."<<endl;
                        secondBorderSelectionPoint = selectedVertex;
                        selectedVertex -> selected = true;
                        selectedVertices.push_back(selectedVertex);
                    }
                }
            } else if(!vertToSelect.empty()) {
                cout<<"Unselecting all points on partial border."<<endl;
                vector<Vertex*>::iterator vIt;
                for(vIt = vertToSelect.begin(); vIt < vertToSelect.end(); vIt++) {
                    (*vIt) -> selected = false;
                }
                firstBorderSelectionPoint -> selected = false;
                secondBorderSelectionPoint -> selected = false;
                firstBorderSelectionPoint = NULL;
                secondBorderSelectionPoint = NULL;
                allBorderPoints.clear();
                vertToSelect.clear();
                selectedVertices.clear();
            } else {
                vector<Vertex*>::iterator vIt;
                bool hasSeenSecondPoint = false;
                for(vIt = allBorderPoints.begin(); vIt < allBorderPoints.end(); vIt++) {
                    if((*vIt) == selectedVertex) {
                        if((*vIt) == firstBorderSelectionPoint
                         || (*vIt) == secondBorderSelectionPoint) {
                            cout<<"Unselecting points on border."<<endl;
                            firstBorderSelectionPoint -> selected = false;
                            secondBorderSelectionPoint -> selected = false;
                            firstBorderSelectionPoint = NULL;
                            secondBorderSelectionPoint = NULL;
                            allBorderPoints.clear();
                            selectedVertices.clear();
                        }
                        break;
                    } else if((*vIt) == secondBorderSelectionPoint) {
                        hasSeenSecondPoint = true;
                    }
                }
                if(!allBorderPoints.empty()) {
                    if(vIt == allBorderPoints.end()) {
                        cout<<"Your choice of the third points is not on"
                         <<"the same border. Selection ABORT."<<endl;
                        firstBorderSelectionPoint -> selected = false;
                        secondBorderSelectionPoint -> selected = false;
                        firstBorderSelectionPoint = NULL;
                        secondBorderSelectionPoint = NULL;
                        allBorderPoints.clear();
                        selectedVertices.clear();
                    } else {
                        cout<<"Selecting this half border on this side."<<endl;
                        vector<Vertex*>::iterator vSelIt;
                        if(!hasSeenSecondPoint) {
                            for(vSelIt = allBorderPoints.begin();
                             vSelIt < allBorderPoints.end(); vSelIt++) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBorderSelectionPoint) {
                                    break;
                                }
                            }
                        } else {
                            vertToSelect.push_back(firstBorderSelectionPoint);
                            for(vSelIt = allBorderPoints.end() - 1;
                             vSelIt >= allBorderPoints.begin(); vSelIt--) {
                                vertToSelect.push_back(*vSelIt);
                                if((*vSelIt) == secondBorderSelectionPoint) {
                                    break;
                                }
                            }
                        }
                        selectedVertices.clear();
                        for(vSelIt = vertToSelect.begin();
                         vSelIt < vertToSelect.end(); vSelIt++) {
                            (*vSelIt) -> selected = true;
                            selectedVertices.push_back(*vSelIt);
                        }
                        cout<<"You have selected "<<selectedVertices.size()
                        <<" points on this border.\n";
                    }
                }
            }
        }
    }
}

void clearSelection() {
    vector<Vertex*>::iterator vIt;
    for(vIt = selectedVertices.begin(); vIt < selectedVertices.end(); vIt++) {
        (*vIt) -> selected = false;
    }
    selectedVertices.clear();
    firstBorderSelectionPoint = NULL;
    secondBorderSelectionPoint = NULL;
    allBorderPoints.clear();
    vertToSelect.clear();
}

//************************************************************
//          Process C. ADDING TO TEMP MESH
//************************************************************

void clearTempMesh() {
    temp_mesh.vertList.clear();
    temp_mesh.faceList.clear();
    temp_mesh.edgeTable.clear();
    border1 = NULL;
    border2 = NULL;
}

void clearPolyline(int index) {
    if(index == 1) {
        border1 -> vertices.clear();
        border1 = NULL;
    } else if (index == 2) {
        border2 -> vertices.clear();
        border2 = NULL;
    } else {
        border1 -> vertices.clear();
        border1 = NULL;
        border2 -> vertices.clear();
        border2 = NULL;
    }
}

void addSelectedToPolyline(int index, bool isLoop = false) {
    vector<Vertex*>::iterator vIt;
    if(index == 1) {
        border1 = new Polyline;
        for(vIt = selectedVertices.begin();
         vIt < selectedVertices.end(); vIt ++) {
            border1 -> vertices.push_back(*vIt);
        }
        border1 -> isLoop = isLoop;
    } else {
        border2 = new Polyline;
        for(vIt = selectedVertices.begin();
         vIt < selectedVertices.end(); vIt ++) {
            border2 -> vertices.push_back(*vIt);
        }
        border2 -> isLoop = isLoop;
    }
    clearSelection();
}

void zipping(int mode = 0, float penalty = 2.0) {
    Zipper * zipper = new Zipper;
    if(mode == 0) {
        temp_mesh = zipper -> zip(border1, border2, penalty);
    } else {
        temp_mesh = zipper -> zip_skewness(border1, border2, penalty);
    }
}

void addSelectedToTempMesh() {
    vector<Vertex*> vertices;
    vector<Vertex*>::iterator vIt;
    for(vIt = selectedVertices.begin();
     vIt < selectedVertices.end(); vIt++) {
        temp_mesh.addVertex(*vIt);
        vertices.push_back(*vIt);
    }
    temp_mesh.addPolygonFace(vertices);
    temp_mesh.buildBoundary();
    temp_mesh.computeNormals();
    clearSelection();
}

//************************************************************
//          Process D. JOINNING TEMP MESH TO MASTER MESH
//************************************************************

void joinForNewMaster() {
    Mesh newMesh;
    vector<Vertex*>::iterator vIt;
    for(vIt = master_mesh.vertList.begin();
        vIt < master_mesh.vertList.end(); vIt ++) {
        Vertex * vertCopy = new Vertex;
        vertCopy -> ID = (*vIt) -> ID;
        vertCopy -> position = (*vIt) -> position;
        newMesh.addVertex(vertCopy);
    }
    vector<Face*>::iterator fIt;
    for(fIt = master_mesh.faceList.begin();
     fIt < master_mesh.faceList.end(); fIt ++) {
        Face * tempFace = *fIt;
        Edge * firstEdge = tempFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        Vertex * tempv;
        vertices.clear();
        do {
            if(tempFace == currEdge -> fa) {
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
            vertices.push_back(newMesh.vertList[tempv -> ID]);
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        newMesh.addPolygonFace(vertices);
    }
    cout<<"Joining Temp Mesh into Master Mesh.\n";
    for(fIt = temp_mesh.faceList.begin();
     fIt < temp_mesh.faceList.end(); fIt ++) {
        Face * tempFace = *fIt;
        Edge * firstEdge = tempFace -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        Vertex * tempv;
        vertices.clear();
        do {
            if(tempFace == currEdge -> fa) {
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
            vertices.push_back(newMesh.vertList[tempv -> ID]);
            currEdge = nextEdge;
        } while (currEdge != firstEdge);
        newMesh.addPolygonFace(vertices);
    }
    newMesh.object2world = master_mesh.object2world;
    master_mesh = newMesh;
    master_mesh.buildBoundary();
    master_mesh.computeNormals();
}

//************************************************************
//          Process E. SUBDIVIDE MASTER MESH
//************************************************************

void subdivide(int level) {
    Subdivision myCC(master_mesh);
    master_mesh = myCC.ccSubdivision(level);
    master_mesh.computeNormals();
    if(copy_mesh.vertList.size() > 0) {
        Subdivision myCC(copy_mesh);
        copy_mesh = myCC.ccSubdivision(level);
        copy_mesh.computeNormals();
    }
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
/*
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

void drawVertices() {
    vector<Vertex*> vertList = master_mesh.vertList;
    vector<Vertex*>::iterator vIt;
    glPointSize(10);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    int counter = 0;
    for(vIt = vertList.begin(); vIt < vertList.end(); vIt++) {
        if((*vIt) -> selected) {
            glBegin(GL_POINTS);
            vec3 position = (*vIt) -> position;
            glNormal3f(position[0] * 100, position[1] * 100, position[2] * 100);
            glVertex3f(position[0], position[1], position[2]);
            glEnd();
        }
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, YELLOW);
}

void render(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0, 0, cameraDistance, 0, 0, 0, 0, 1, 0);

    glMultMatrixf(&master_mesh.object2world[0][0]);
    //drawVertices();
    //glLoadName(INT_MAX);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, CYAN);
    master_mesh.drawMesh();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, PURPLE);
    if(!temp_mesh.vertList.empty()) {
        temp_mesh.drawMesh();
    }
    if(!copy_mesh.vertList.empty()) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BLUE);
        copy_mesh.drawMesh();
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, RED);
    glLineWidth(4.0);
    if(border1) {
        border1 -> drawLine();
    }
    if(border2) {
        border2 -> drawLine();
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, YELLOW);
    drawVertices();

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
        glm::mat3 camera2object =
         inverse(mat3(transforms[MODE_CAMERA]) * mat3(master_mesh.object2world));
        glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        master_mesh.object2world =
         rotate(master_mesh.object2world, (float) 1.0 * angle, axis_in_object_coord);
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
    // Selection
    if (key == 'b') {
        cout<<"Change Selection MODE. All Selected Vertices clear.\n";
        clearSelection();
        if (borderSelection == 2) {
            cout<<"Border Selection Switch OFF."<<endl;
            borderSelection = 0;
        } else if (borderSelection == 0) {
            cout<<"Whole Border Selection Switch ON."<<endl;
            borderSelection = 1;
        } else {
            cout<<"Partial Border Selection Switch ON. (Whole Border Selection OFF.)"<<endl;
            borderSelection = 2;
        }
    }
    if (key == 'u') {
        cout<<"Unselecting all points.\n";
        clearSelection();
    }
    // Adding
    if (key == 'm') {
        if (addingMode == 0) {
            cout<<"Switch to Zipping Mode (Line Strip).\n";
            addingMode = 1;
        } else if(addingMode == 1) {
            cout<<"Switch to Zipping Mode (Line Loop).\n";
            addingMode = 2;
        } else if(addingMode == 2) {
            cout<<"Switch to Adding Mode.\n";
            addingMode = 0;
        }
    }
    if (key == 'a') {
        if(!selectedVertices.empty()) {
            if(addingMode == 0) {
                cout<<"Adding selected vertices as a face to temp mesh.\n";
                addSelectedToTempMesh();
            } else if (addingMode == 1) {
                cout<<"Make selected vertices as one polyline Strip for zipping.\n";
                if(border1 == NULL) {
                    addSelectedToPolyline(1);
                } else if (border2 == NULL) {
                    addSelectedToPolyline(2);
                } else {
                    cout<<"Two borders are selected."
                    <<" Please zip them before adding other polylines.\n";
                }
            } else {
                cout<<"Make selected vertices as one polyline Loop for zipping.\n";
                if(border1 == NULL) {
                    addSelectedToPolyline(1, true);
                } else if (border2 == NULL) {
                    addSelectedToPolyline(2, true);
                } else {
                    cout<<"Two borders are selected."
                    <<" Please zip them before adding other polylines.\n";
                }
            }
        } else {
            cout<<"No vertices selected. Please select vertices first.\n";
        }
    }
    if (key == 'z') {
        if(addingMode == 1 || addingMode == 2) {
            cout<<"Zipping two border polylines.\n";
            if(border1 == NULL || border2 == NULL) {
                cout<<"You need to select two borders to zip.\n";
            } else {
                cout<<"Zipping two borders.\n";
                zipping();
                border1 = NULL;
                border2 = NULL;
            }
        }
    }
    if (key == 'c') {
        clearTempMesh();
    }
    if (key == 'j') {
        joinForNewMaster();
        clearTempMesh();
    }
    if (key == 'd') {
        subdivide(1);
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
        Face * workFace = master_mesh.faceList[selectedID];
        if(!workFace->selected) {
            workFace->selected = true;
        } else {
            workFace->selected = false;
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
    gluUnProject( winX, winY, winZ, modelview, projection,
     view, &posX, &posY, &posZ);
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
    if(borderSelection == 0){
        selectSinglePoint(hits, buff, posX, posY, posZ);
    } else if( borderSelection == 1) {
        selectWholeBorder(hits, buff, posX, posY, posZ);
    } else {
        selectPartialBorder(hits, buff, posX, posY, posZ);
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

