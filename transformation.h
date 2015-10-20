// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <vector>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Transformation Class -- A class to create a new mesh by transformation.
    
// Create a new Mesh given the transformation matrix
// @param matrix, the mesh of self-merege
Mesh transform(Mesh & mesh, mat4 matrix) {
    Mesh transfromedMesh;
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    unordered_map<Vertex*, Vertex*> transformMap;
    unordered_map<Vertex*, Vertex*>::iterator mIt;
    for(vIt = mesh.vertList.begin(); vIt < mesh.vertList.end(); vIt ++) {
        Vertex * v = new Vertex;
        v -> position = ((*vIt) -> position);
        v -> ID = mergedMesh.vertList.size();
        transformMap[*vIt] = v;
        mergedMesh.vertList.push_back(v);
    }
    for(fIt = mesh1.faceList.begin(); fIt < mesh1.faceList.end(); fIt ++) {
        Edge * firstEdge = (*fIt) -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        //vertices.clear();
        do{
            if(currEdge -> fa == (*fIt)) {
                vertices.push_back(mergeMap[currEdge -> vb]);
                nextEdge = currEdge -> nextVbFa;
            } else{
                if(currEdge -> mobius) {
                    vertices.push_back(mergeMap[currEdge -> vb]);
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    vertices.push_back(mergeMap[currEdge -> va]);
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            currEdge = nextEdge;
        } while(currEdge != firstEdge);
        mergedMesh.addPolygonFace(vertices);
    }
    for(fIt = mesh2.faceList.begin(); fIt < mesh2.faceList.end(); fIt ++) {
        Edge * firstEdge = (*fIt) -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        //vertices.clear();
        do{
            if(currEdge -> fa == (*fIt)) {
                vertices.push_back(mergeMap[currEdge -> vb]);
                nextEdge = currEdge -> nextVbFa;
            } else{
                if(currEdge -> mobius) {
                    vertices.push_back(mergeMap[currEdge -> vb]);
                    nextEdge = currEdge -> nextVbFb;
                } else {
                    vertices.push_back(mergeMap[currEdge -> va]);
                    nextEdge = currEdge -> nextVaFb;
                }
            }
            currEdge = nextEdge;
        } while(currEdge != firstEdge);
        mergedMesh.addPolygonFace(vertices);
    }
    mergedMesh.buildBoundary();
    return mergedMesh;
}


#endif // __TRANSFORMATION_H__