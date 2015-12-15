// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __SELECTION_H__
#define __SELECTION_H__

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Selection Class -- Handles Mouse Selection.

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

#endif // __SELECTION_H__