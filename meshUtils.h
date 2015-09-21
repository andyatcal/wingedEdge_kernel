// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __MESHUTILS_H__
#define __MESHUTILS_H__

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

// Forward declarations
class Vertex;
class Face;
class Halfedge;
class Mesh;

//////////////////////////////////////////////////////////////////////
// MeshUtils Class -- Utility Functions with Mesh.

// Flush mesh. Make the vertTable, edgeTable, faceTable of a mesh empty.
// @param mesh : the mesh to flush.
void flushMesh(Mesh &mesh) {
    vector<Face*> faceVect = mesh.faceVect;
    unordered_map<Vertex*, vector<Halfedge*> > boundaryEdgeTable = mesh.boundaryEdgeTable;
    unordered_map<unsigned long, Vertex*> vertTable = mesh.vertTable;
    Vertex * tempVert;
    vector<Halfedge*> tempEdgeVect;
    Face * tempFace;
    vector<Face*>::iterator fIt;
    unordered_map<Vertex*, vector<Halfedge*> >::iterator eIt;
    unordered_map<unsigned long, Vertex*>::iterator vIt;
    for(fIt = faceVect.begin(); fIt < faceVect.end(); fIt++) {
        tempFace = (*fIt);
        delete tempFace;
    }
    for(eIt = boundaryEdgeTable.begin(); eIt != boundaryEdgeTable.end(); eIt++) {
        tempEdgeVect = eIt -> second;
        tempEdgeVect.clear();
    }
    for(vIt = vertTable.begin(); vIt != vertTable.end(); vIt++) {
        tempVert = vIt -> second;
        delete tempVert;
    }
    faceVect.clear();
    boundaryEdgeTable.clear();
    vertTable.clear();
}

// Add a vertex to mesh.
void addVertexToMesh(Vertex * vertex, Mesh &mesh) {
    unordered_map<unsigned long, Vertex*> & vertTable = mesh.vertTable;
    unsigned long ID = vertex -> ID;
    if(vertTable.find(ID) == vertTable.end()) {
        vertTable[ID] = vertex;
        vector<Halfedge*> boundaryEdgesAtThisPoint;
        boundaryEdgesAtThisPoint.clear();
        mesh.boundaryEdgeTable[vertex] = boundaryEdgesAtThisPoint;
    } else {
        cout<<"HEY, YOU ARE ADDING ANOTHER VERTEX WITH SAME ID!"<<endl;
        exit(0);
    }
}

// Add an edge to mesh. Find its sibling in existing boundary edges. If not found, add it to the boundary.
void addEdgeToMesh(Halfedge * edge, Mesh & mesh) {
    unordered_map<Vertex*, vector<Halfedge*> > &boundaryEdgeTable = mesh.boundaryEdgeTable;
    unordered_map<Vertex*, vector<Halfedge*> >::iterator evIt;
    vector<Halfedge*>::iterator eIt;
    Vertex * start = edge -> start;
    Vertex * end = edge -> end;
    bool siblingFound = false;
    bool mobiusSiblingFound = false;
    evIt = boundaryEdgeTable.find(end);
    vector<Halfedge*> & boundaryEdgesAtEnd = evIt -> second;
    //cout<<"This edge -> end has "<<boundaryEdgesAtEnd.size()<<" temp boundaries."<<endl;
    for(eIt = boundaryEdgesAtEnd.begin(); eIt < boundaryEdgesAtEnd.end(); eIt++) {
        //cout<<"Testing: this test edge start with ID "<<(*eIt) -> start -> ID<<" and end with ID "<< (*eIt) -> end -> ID<<endl;
        if((*eIt) -> end == start) {
            edge -> sibling = (*eIt);
            (*eIt) -> sibling = edge;
            siblingFound = true;
            boundaryEdgesAtEnd.erase(eIt);
            //cout<<"sibling found!"<<endl;
            break;
        }
    }
    if(!siblingFound) {
        evIt = boundaryEdgeTable.find(start);
        vector<Halfedge*> & boundaryEdgesAtStart = evIt -> second;
        for(eIt = boundaryEdgesAtStart.begin(); eIt < boundaryEdgesAtStart.end(); eIt++) {
            if((*eIt) -> end == end) {
                edge -> mobiusSibling = (*eIt);
                (*eIt) -> mobiusSibling = edge;
                start -> onMobiusSibling = true;
                end -> onMobiusSibling = true;
                mobiusSiblingFound = true;
                boundaryEdgesAtStart.erase(eIt);
                //cout<<"mobius sibling found!"<<endl;
                break;
            }
        }
        if((!siblingFound) && (!mobiusSiblingFound)) {
            boundaryEdgesAtStart.push_back(edge);
        }
    }
}
// Add a triangle face with three vertex to a mesh.
// @param va, vb, vc: three vertices to construct the face.
// @param mesh: a reference to the mesh.
void addTriFaceToMesh(Vertex * va, Vertex* vb, Vertex * vc, Mesh & mesh) {
    Face * currFace = new Face;
    Halfedge *e1, *e2, *e3;
    e1 = new Halfedge(va, vb);
    e2 = new Halfedge(vb, vc);
    e3 = new Halfedge(vc, va);
    e1 -> next = e2;
    e2 -> next = e3;
    e3 -> next = e1;
    e2 -> previous = e1;
    e3 -> previous = e2;
    e1 -> previous = e3;
    e1 -> heFace = currFace;
    e2 -> heFace = currFace;
    e3 -> heFace = currFace;
    currFace -> oneSideEdge = e3;
    mesh.faceVect.push_back(currFace);
    addEdgeToMesh(e1, mesh);
    addEdgeToMesh(e2, mesh);
    addEdgeToMesh(e3, mesh);
}

// Add a quad face with three vertex to a mesh.
// @param va, vb, vc, vd: four vertices to construct the face.
// @param mesh: a reference to the mesh.
void addQuadFaceToMesh(Vertex * va, Vertex* vb, Vertex * vc, Vertex * vd, Mesh & mesh) {
    Face * currFace = new Face;
    Halfedge *e1, *e2, *e3, *e4;
    e1 = new Halfedge(va, vb);
    e2 = new Halfedge(vb, vc);
    e3 = new Halfedge(vc, vd);
    e4 = new Halfedge(vd, va);
    e1 -> next = e2;
    e2 -> next = e3;
    e3 -> next = e4;
    e4 -> next = e1;
    e2 -> previous = e1;
    e3 -> previous = e2;
    e4 -> previous = e3;
    e1 -> previous = e4;
    e1 -> heFace = currFace;
    e2 -> heFace = currFace;
    e3 -> heFace = currFace;
    e4 -> heFace = currFace;
    currFace -> oneSideEdge = e4;
    mesh.faceVect.push_back(currFace);
    addEdgeToMesh(e1, mesh);
    addEdgeToMesh(e2, mesh);
    addEdgeToMesh(e3, mesh);
    addEdgeToMesh(e4, mesh);
}

// Add a polygon face with N vertex to a mesh.
// @param vertices: a list of points to construct the face.
// @param mesh: a reference to the mesh.
// @param reverseOrder: flag, true if we add the vertices in the reverse order.
void addPolygonFaceToMesh(vector<Vertex*> vertices, Mesh & mesh,
                bool reverseOrder = false) {
    Face * currFace = new Face;
    vector<Halfedge*> edges;
    vector<Vertex*>::iterator vIt;
    Vertex * currVert;
    Halfedge * currEdge;
    if(!reverseOrder) {
        for(vIt = vertices.begin(); vIt < vertices.end(); vIt++) {
            currVert = *vIt;
            Halfedge * tempEdge;
            if(vIt == (vertices.end() - 1)) {
                tempEdge = new Halfedge(*vIt, *(vertices.begin()));
            } else {
                tempEdge = new Halfedge(*vIt, *(vIt + 1));
            }
            edges.push_back(tempEdge);
        }
    } else {
        for(vIt = vertices.end() - 1; vIt >= vertices.begin(); vIt--) {
            currVert = *vIt;
            Halfedge * tempEdge;
            if(vIt == (vertices.end() - 1)) {
                tempEdge = new Halfedge(*(vertices.begin()), *vIt);
            } else {
                tempEdge = new Halfedge(*(vIt + 1), *vIt);
            }
            edges.push_back(tempEdge);
        }
    }
    vector<Halfedge*>::iterator eIt;
    for(eIt = edges.begin(); eIt < edges.end(); eIt++) {
        currEdge = *eIt;
        if(eIt == (edges.end() - 1)) {
            currEdge -> next = *(edges.begin());
            Halfedge * temp = *(edges.begin());
            temp -> previous = currEdge;
        } else {
            currEdge -> next = *(eIt + 1);
            Halfedge * temp = *(eIt + 1);
            temp -> previous = currEdge;
        }
        currEdge -> heFace = currFace;
        addEdgeToMesh(currEdge, mesh);
    }
    currFace -> oneSideEdge = *(edges.begin());
    mesh.faceVect.push_back(currFace);
}

// Given the list of boundary edges, build the connections between halfedges.
// @param mesh: refer to the mesh to build connection in.
// This one takes O(E) time.
void buildBoundaryPointers(Mesh &mesh) {
    unordered_map<Vertex*, vector<Halfedge*> >
     &boundaryEdgeTable = mesh.boundaryEdgeTable;
    unordered_map<Vertex*, vector<Halfedge*> >::iterator evIt;
    vector<Halfedge*> boundaryEdgesAtThisPoint;
    vector<Halfedge*>::iterator eIt;
    for(evIt = boundaryEdgeTable.begin(); evIt
     != boundaryEdgeTable.end(); evIt++) {
        boundaryEdgesAtThisPoint = evIt -> second;
        if(!boundaryEdgesAtThisPoint.empty()) {
            for(eIt = boundaryEdgesAtThisPoint.begin();
             eIt < boundaryEdgesAtThisPoint.end(); eIt++) {
                Halfedge * currEdge = (*eIt);
                if(currEdge -> previousBoundary == NULL
                 && currEdge -> mobiusBoundary == NULL) {
                    int mobiusCounter = 0;
                    Halfedge * firstBoundaryEdge = currEdge;
                    //cout<<"first: "<<currEdge -> start -> ID<<" "<<currEdge -> end -> ID<<endl;
                    do {
                        //cout<<"mobius counter: " << mobiusCounter<<endl;
                        Halfedge * nextBoundaryEdge;
                        currEdge -> isSharp = true;
                        //cout<<currEdge -> start -> ID<<" "<<currEdge -> end -> ID<<endl;
                        if(mobiusCounter % 2 == 0) {
                            nextBoundaryEdge = currEdge -> next;
                        } else {
                            nextBoundaryEdge = currEdge -> previous;
                        }
                        while(nextBoundaryEdge -> sibling != NULL
                         || nextBoundaryEdge -> mobiusSibling != NULL) {
                            if(nextBoundaryEdge -> sibling != NULL) {
                                nextBoundaryEdge = nextBoundaryEdge -> sibling;
                            } else {
                                mobiusCounter += 1;
                                nextBoundaryEdge = nextBoundaryEdge -> mobiusSibling;
                            }
                            if(mobiusCounter % 2 == 0) {
                                nextBoundaryEdge = nextBoundaryEdge -> next;
                            } else {
                                nextBoundaryEdge = nextBoundaryEdge -> previous;
                            }
                        }
                        //cout<<"CurrEdge: "<<currEdge -> start -> ID<<" "<<currEdge -> end -> ID<<endl;
                        //cout<<"nextBoundaryEdge: "<<nextBoundaryEdge -> start -> ID<<" "<<nextBoundaryEdge -> end -> ID<<endl;
                        if(nextBoundaryEdge -> start == currEdge -> end) {
                            currEdge -> nextBoundary = nextBoundaryEdge;
                            nextBoundaryEdge -> previousBoundary = currEdge;
                        } else if(nextBoundaryEdge -> end == currEdge -> start) {
                            currEdge -> previousBoundary = nextBoundaryEdge;
                            nextBoundaryEdge -> nextBoundary = currEdge;
                        } else if(nextBoundaryEdge -> start == currEdge -> start
                         || nextBoundaryEdge -> end == currEdge -> end) {
                            currEdge -> mobiusBoundary = nextBoundaryEdge;
                            nextBoundaryEdge -> mobiusBoundary = currEdge;
                        } else {
                            cout<<"ERROR: DEBUG the boundary builder"<<endl;
                            exit(0);
                        }
                        currEdge = nextBoundaryEdge;
                    } while (currEdge != firstBoundaryEdge);
                }
            }
        }
    }
}

// Build connnections after making the faces.
// @param mesh: reference to the mesh that the connections located in.
void buildConnections(Mesh &mesh) {
    //buildSiblingPointers(mesh);
    buildBoundaryPointers(mesh);
    //buildBoundaryPointers2(mesh);
    
}

// Return the normal at point v2. 
// @param p1, p2, p3 are positions of three vertices,
// with edge p1 -> p2 and edge p2 -> p3.
vec3 getNormal3Vertex(vec3 p1, vec3 p2, vec3 p3){
    return normalize(cross(p2 - p1, p3 - p2));
}

// Get the normal at the end of a halfedge. 
// @param currEdge: pointer of the edge, which the vertex on the end of.
vec3 getEndOfEdgeNormal(Halfedge * currEdge){
    Vertex * v1 = currEdge -> start;
    Vertex * v2 = currEdge -> end;
    Vertex * v3 = currEdge -> next -> end;
    return getNormal3Vertex(v1 -> position, v2 -> position, v3 -> position);
}

// Get the surface normal. 
// @param currFace: pointer of the face.
void getFaceNormal(Face * currFace){
    Halfedge * firstEdge = currFace -> oneSideEdge;
    Halfedge * currEdge;
    currEdge = firstEdge;
    vec3 avgNorm(0, 0, 0);
    do {
        avgNorm += getEndOfEdgeNormal(currEdge);
        currEdge = currEdge -> next;
    } while (currEdge != firstEdge);
    vec3 result = normalize(avgNorm);
    currFace -> faceNormal = normalize(avgNorm);;
}

// Get a triangular surface normal.
// @param v1 v2 v3: three vertices of the triangle.
vec3 getTriFaceNormal(Vertex * va, Vertex * vb, Vertex * vc){
    vec3 pa = va -> position;
    vec3 pb = vb -> position;
    vec3 pc = vc -> position;
    vec3 vNormalb = getNormal3Vertex(pa, pb, pc);
    vec3 vNormalc = getNormal3Vertex(pb, pc, pa);
    vec3 vNormala = getNormal3Vertex(pc, pa, pb);
    return normalize(vNormala + vNormalb + vNormalc);
}

// Get the vertex normal
// @param currVert: pointer of the edge, which the vertex on the end of.
void getVertexNormal(Vertex * currVert){
    Halfedge * firstOutEdge;
    Halfedge * nextOutEdge;
    firstOutEdge = currVert -> oneOutEdge;
    nextOutEdge = firstOutEdge;
    vec3 avgNorm(0, 0, 0);
    int mobiusCounter = 0;
    do {
        //cout<<"The value of avgNorm is :"<<avgNorm[0]<<" "<<avgNorm[1]<<" "<<avgNorm[2]<<endl;
        if(mobiusCounter % 2 == 0) {
            avgNorm += nextOutEdge -> heFace -> faceNormal;
        } else {
            avgNorm -= nextOutEdge -> heFace -> faceNormal;
        }
        if(nextOutEdge -> sibling != NULL) {
            if(mobiusCounter % 2 == 0) {
                nextOutEdge = nextOutEdge -> sibling -> next;
            } else {
                nextOutEdge = nextOutEdge -> sibling -> previous;
            }
        } else if(nextOutEdge -> mobiusSibling != NULL) {
            mobiusCounter += 1;
            if(mobiusCounter % 2 == 0) {
                nextOutEdge = nextOutEdge -> mobiusSibling -> next;
            } else {
                nextOutEdge = nextOutEdge -> mobiusSibling -> previous;
            }
        } else if(!(currVert -> onMobiusSibling)) {
            if(mobiusCounter % 2 == 0) {
                nextOutEdge = nextOutEdge -> previousBoundary -> next;
            } else {
                nextOutEdge = nextOutEdge -> previousBoundary -> previous;
            }
        } else {
            mobiusCounter += 1;
            if(mobiusCounter % 2 == 0) {
                nextOutEdge = nextOutEdge -> mobiusBoundary -> next;
            } else {
                nextOutEdge = nextOutEdge -> mobiusBoundary -> previous;
            }
        }
    } while ( nextOutEdge != firstOutEdge);
    //cout<<"The value of avgNorm is :"<<avgNorm[0]<<" "<<avgNorm[1]<<" "<<avgNorm[2]<<endl;
    //cout<<"ID: "<<currVert -> ID <<" has "<<mobiusCounter<<" mConter"<<endl;
    currVert -> normal = normalize(avgNorm);
}

// Iterate over every vertex in the mesh and compute its normal
// @param vertTable: the vector of vertex to compute normals.
void computeNormals(Mesh & mesh){
    unordered_map<unsigned long, Vertex*> & vertTable = mesh.vertTable;
    unordered_map<unsigned long, Vertex*>::iterator vIt;
    vector<Face*> & faceVect = mesh.faceVect;
    vector<Face*>::iterator fIt;
    //cout<<"faceTable size: "<<faceVect.size()<<endl;
    for(fIt = faceVect.begin(); fIt < faceVect.end(); fIt++){
        getFaceNormal(*fIt);
    }
    //cout<<"the vertTable size is "<<vertTable.size()<<endl;
    for(vIt = vertTable.begin(); vIt != vertTable.end(); vIt++){
        //cout<<"Now calculating vertex with ID: "<< vIt -> first <<endl;
        getVertexNormal(vIt -> second);
    }
}

// Draw a mesh in OpenGL with GL_POLYGON
// @param mesh: reference to the mesh to draw.
void drawMesh(Mesh & mesh) {
    Face * tempFace;
    vector<Face*>::iterator fIt;
    //cout<<endl;
    for(fIt = mesh.faceVect.begin(); fIt < mesh.faceVect.end(); fIt++){
        tempFace = (*fIt);
        vec3 fNormal = tempFace -> faceNormal;
        Vertex * tempv;
        Halfedge * firstSideEdge = (*fIt) -> oneSideEdge;
        //cout<<"New Face: "<<endl;
        glBegin(GL_POLYGON);
        Halfedge * nextSideEdge = firstSideEdge;
        do {
            tempv = nextSideEdge -> end;
            float normx;
            float normy;
            float normz;
            if(tempv -> onMobiusSibling) {
                vec3 vNormal = tempv -> normal;
                if(dot(vNormal, fNormal) >= 0) {
                    normx = tempv -> normal[0];
                    normy = tempv -> normal[1];
                    normz = tempv -> normal[2];
                } else {
                    normx = - tempv -> normal[0];
                    normy = - tempv -> normal[1];
                    normz = - tempv -> normal[2];
                }
            } else {
                normx = tempv -> normal[0];
                normy = tempv -> normal[1];
                normz = tempv -> normal[2];
            }
            //cout<<"normx: "<<normx<<" normy: "<<normy<<" normz: "<<normz<<endl;
            glNormal3f(normx, normy, normz);
            float x = tempv -> position[0];
            float y = tempv -> position[1];
            float z = tempv -> position[2];
            //cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<endl;
            glVertex3f(x, y, z);
            nextSideEdge = nextSideEdge -> next;
        } while(nextSideEdge != firstSideEdge);
        glEnd();
    }
}
// merge edge 2 to edeg 1
void mergeEdges(Halfedge * edge1, Halfedge * edge2, bool reverse = false) {
    if(!reverse) {
        edge2 -> start = edge1 -> start;
        edge2 -> end = edge1 -> end;
        edge2 -> previous -> end = edge1 -> start;
        edge2 -> next -> start = edge1 -> end;
        edge1 -> start -> onMobiusSibling = true;
        edge1 -> end -> onMobiusSibling = true;
    } else {
        edge2 -> start = edge1 -> end;
        edge2 -> end = edge1 -> start;
        edge2 -> previous -> end = edge1 -> end;
        edge2 -> next -> start = edge1 -> start;
    }
}

// merge a sequnce of coinciding vertices on the boundary.
void mergeSelfBoundary(Mesh &mesh) {
    unordered_map<Vertex*, vector<Halfedge*> > &boundaryEdgeTable = mesh.boundaryEdgeTable;
    unordered_map<Vertex*, vector<Halfedge*> >::iterator evIt;
    vector<Halfedge*> boundaryEdgeVect;
    vector<Halfedge*>::iterator eIt;
    for(evIt = boundaryEdgeTable.begin(); evIt != boundaryEdgeTable.end(); evIt++) {
        vector<Halfedge*> boundaryAtVertex = evIt -> second;
        for(eIt = boundaryAtVertex.begin(); eIt < boundaryAtVertex.end(); eIt++) {
            boundaryEdgeVect.push_back(*eIt);
        }
    }
    vector<Halfedge*>::iterator eIt1;
    vector<Halfedge*>::iterator eIt2;
    unordered_map<Vertex *, Vertex *> replaceMap;
    unordered_map<Vertex *, Vertex *>::iterator rpIt;
    replaceMap.clear();
    for(eIt1 = boundaryEdgeVect.begin(); eIt1 < boundaryEdgeVect.end(); eIt1++) {
        Halfedge * edge1 = (*eIt1);
        for(eIt2 = eIt1 + 1; eIt2 < boundaryEdgeVect.end(); eIt2++) {
            Halfedge * edge2 = (*eIt2);
            if(distance(edge1 -> start -> position, edge2 -> start -> position) < 0.0001
             && distance(edge1 -> end -> position, edge2 -> end -> position) < 0.0001) {
                evIt = boundaryEdgeTable.find(edge1 -> start);
                vector<Halfedge*> & boundaryAtVertexA = evIt -> second;
                for(eIt = boundaryAtVertexA.begin(); eIt < boundaryAtVertexA.end(); eIt++) {
                    if((*eIt) == edge1) {
                        boundaryAtVertexA.erase(eIt);
                        break;
                    }
                }
                evIt = boundaryEdgeTable.find(edge2 -> start);
                vector<Halfedge*> & boundaryAtVertexB = evIt -> second;
                for(eIt = boundaryAtVertexB.begin(); eIt < boundaryAtVertexB.end(); eIt++) {
                    if((*eIt) == edge2) {
                        boundaryAtVertexB.erase(eIt);
                        break;
                    }
                }
                edge2 -> mobiusSibling = edge1;
                edge1 -> mobiusSibling = edge2;
                rpIt = replaceMap.find(edge1 -> start);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge2 -> start) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge1 -> end] = edge2 -> end;
                        mergeEdges(edge2, edge1);
                        break;
                    }
                }
                rpIt = replaceMap.find(edge1 -> end);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge2 -> end) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge1 -> start] = edge2 -> start;
                        mergeEdges(edge2, edge1);
                        break;
                    }
                }
                rpIt = replaceMap.find(edge2 -> start);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge1 -> start) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge2 -> end] = edge1 -> end;
                        mergeEdges(edge1, edge2);
                        break;
                    }
                }
                rpIt = replaceMap.find(edge2 -> end);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge1 -> end) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge2 -> start] = edge1 -> start;
                        mergeEdges(edge1, edge2);
                        break;
                    }
                }
                if(edge1 -> start -> ID + edge1 -> end -> ID <
                 edge2 -> start -> ID + edge2 -> end -> ID) {
                    replaceMap[edge2 -> start] = edge1 -> start;
                    replaceMap[edge2 -> end] = edge1 -> end;
                    mergeEdges(edge1, edge2);
                } else {
                    replaceMap[edge1 -> start] = edge2 -> start;
                    replaceMap[edge1 -> end] = edge2 -> end;
                    mergeEdges(edge2, edge1);
                }
            } else if(distance(edge1 -> start -> position, edge2 -> end -> position) < 0.0001
             && distance(edge1 -> end -> position, edge2 -> start -> position) < 0.0001) {
                evIt = boundaryEdgeTable.find(edge1 -> start);
                vector<Halfedge*> & boundaryAtVertexA = evIt -> second;
                for(eIt = boundaryAtVertexA.begin(); eIt < boundaryAtVertexA.end(); eIt++) {
                    if((*eIt) == edge1) {
                        boundaryAtVertexA.erase(eIt);
                        break;
                    }
                }
                evIt = boundaryEdgeTable.find(edge2 -> start);
                vector<Halfedge*> & boundaryAtVertexB = evIt -> second;
                for(eIt = boundaryAtVertexB.begin(); eIt < boundaryAtVertexB.end(); eIt++) {
                    if((*eIt) == edge2) {
                        boundaryAtVertexB.erase(eIt);
                        break;
                    }
                }
                edge2 -> sibling = edge1;
                edge1 -> sibling = edge2;
                rpIt = replaceMap.find(edge1 -> start);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge2 -> end) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge1 -> end] = edge2 -> start;
                        mergeEdges(edge2, edge1, true);
                        break;
                    }
                }
                rpIt = replaceMap.find(edge1 -> end);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge2 -> start) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge1 -> start] = edge2 -> end;
                        mergeEdges(edge2, edge1, true);
                        break;
                    }
                }
                rpIt = replaceMap.find(edge2 -> start);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge1 -> end) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge2 -> end] = edge1 -> start;
                        mergeEdges(edge1, edge2, true);
                        break;
                    }
                }
                rpIt = replaceMap.find(edge2 -> end);
                if(rpIt != replaceMap.end()) {
                    if(rpIt -> second != edge1 -> start) {
                        cout<<"Error: Non manifold point found. Vertex ID: "
                        << edge1 -> start -> ID<<" .";
                        exit(0);
                    } else {
                        replaceMap[edge2 -> start] = edge1 -> end;
                        mergeEdges(edge1, edge2, true);
                        break;
                    }
                }
                if(edge1 -> start -> ID + edge1 -> end -> ID <
                 edge2 -> start -> ID + edge2 -> end -> ID) {
                    replaceMap[edge2 -> start] = edge1 -> end;
                    replaceMap[edge2 -> end] = edge1 -> start;
                    mergeEdges(edge1, edge2, true);
                } else {
                    replaceMap[edge1 -> start] = edge2 -> end;
                    replaceMap[edge1 -> end] = edge2 -> start;
                    mergeEdges(edge2, edge1, true);
                }
            }
        }

    }
    //Erase the merged vertices in vertable.
    for(rpIt = replaceMap.begin(); rpIt != replaceMap.end(); rpIt++) {
        //cout<<"size of vertable before erase: "<<mesh.vertTable.size()<<endl;
        mesh.vertTable.erase(rpIt -> first -> ID);
        //cout<<"size of vertable after erase: "<<mesh.vertTable.size()<<endl;
    }
}

#endif // __MESHUTILS_H__