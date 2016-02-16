// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __MERGE_H__
#define __MERGE_H__

#include <vector>
#include <unordered_map>

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Merge Class -- A merge class contains fuctions to merge meshes.

// Return true if two vertices are very close to each other.
bool vertexMatch(Vertex * v1, Vertex * v2) {
    float epsilon = 0.001;
    //cout<<"Now matching v1 "<<v1 -> ID<< " and v2 "<< v2 -> ID<<endl;
    //cout<<v1 -> position[0]<<" "<<v1 -> position[1]<<" "<<v1 -> position[2]<<endl;
    //cout<<v2 -> position[0]<<" "<<v2 -> position[1]<<" "<<v2 -> position[2]<<endl;
    //cout<<endl;
    if(distance(v1 -> position, v2 -> position) < epsilon) {
        return true;
    }
    return false;
}

// Merge any possible boundary edges that are close for two meshes.
// @param mesh1, mesh2. The two meshes to be merged.
// Return a new mesh that contains the merged mesh.
Mesh merge(Mesh & mesh1, Mesh & mesh2) {
    unordered_map<Vertex*, Vertex*> replacingMap;
    unordered_map<Vertex*, Vertex*>::iterator mIt;
    vector<Edge*> boundaryEdgeList1 = mesh1.boundaryEdgeList();
    vector<Edge*> boundaryEdgeList2 = mesh2.boundaryEdgeList();
    //cout<<boundaryEdgeList1.size()<<endl;
    //cout<<boundaryEdgeList2.size()<<endl;
    vector<Edge*>::iterator eIt1;
    vector<Edge*>::iterator eIt2;
    for(eIt1 = boundaryEdgeList1.begin(); eIt1 < boundaryEdgeList1.end(); eIt1 ++) {
        for(eIt2 = boundaryEdgeList2.begin(); eIt2 < boundaryEdgeList2.end(); eIt2 ++) {
            if(vertexMatch((*eIt1) -> va, (*eIt2) -> vb) 
                && vertexMatch((*eIt1) -> vb, (*eIt2) -> va)) {
                cout<<"Hey, I find a normal replacing"<<endl;
                cout<<(*eIt1) -> va -> ID<<" matching with "<<(*eIt2) -> vb -> ID<<endl;
                cout<<(*eIt1) -> vb -> ID<<" matching with "<<(*eIt2) -> va -> ID<<endl;
                mIt = replacingMap.find((*eIt2) -> vb);
                if(mIt == replacingMap.end()) {
                    cout<<"A"<<endl;
                    mIt = replacingMap.find((*eIt1) -> va);
                    if(mIt == replacingMap.end()) {
                        cout<<"A1"<<endl;
                        cout<<"matching: "<<(*eIt2) -> vb -> ID<<" to "<<(*eIt1) -> va -> ID<<endl;
                        replacingMap[(*eIt2) -> vb] = (*eIt1) -> va;
                    } else {
                        cout<<"A2"<<endl;
                        cout<<"matching: "<<(*eIt2) -> vb -> ID<<" to "<<replacingMap[(*eIt1) -> va] -> ID<<endl;
                        replacingMap[(*eIt2) -> vb] = replacingMap[(*eIt1) -> va];
                    }
                } else {
                    cout<<"B"<<endl;
                    if(replacingMap[(*eIt2) -> vb] != (*eIt1) -> va) {
                        mIt = replacingMap.find((*eIt1) -> va);
                        if(mIt == replacingMap.end()) {
                            cout<<"B1"<<endl;
                            cout<<"matching: "<<(*eIt1) -> va -> ID<<" to "<<replacingMap[(*eIt2) -> vb] -> ID<<endl;
                            replacingMap[(*eIt1) -> va] = replacingMap[(*eIt2) -> vb];
                        } else {
                            cout<<"B2"<<endl;
                            cout<<"matching: "<<replacingMap[(*eIt2) -> vb] -> ID<<" to "<<replacingMap[(*eIt1) -> va] -> ID<<endl;
                            cout<<"matching: "<<(*eIt2) -> vb -> ID<<" to "<<replacingMap[(*eIt1) -> va] -> ID<<endl;
                            replacingMap[replacingMap[(*eIt2) -> vb]] = replacingMap[(*eIt1) -> va];
                            replacingMap[(*eIt2) -> vb] = replacingMap[(*eIt1) -> va];
                        }
                    }
                }
                mIt = replacingMap.find((*eIt2) -> va);
                if(mIt == replacingMap.end()) {
                    cout<<"A."<<endl;
                    mIt = replacingMap.find((*eIt1) -> vb);
                    if(mIt == replacingMap.end()) {
                        cout<<"A.1"<<endl;
                        cout<<"matching: "<<(*eIt2) -> va -> ID<<" to "<<(*eIt1) -> vb -> ID<<endl;
                        replacingMap[(*eIt2) -> va] = (*eIt1) -> vb;
                    } else {
                        cout<<"A.2"<<endl;
                        cout<<"matching: "<<(*eIt2) -> va -> ID<<" to "<<replacingMap[(*eIt1) -> vb] -> ID<<endl;
                        replacingMap[(*eIt2) -> va] = replacingMap[(*eIt1) -> vb];
                    }
                } else {
                    cout<<"B."<<endl;
                    if(replacingMap[(*eIt2) -> va] != (*eIt1) -> vb) {
                        mIt = replacingMap.find((*eIt1) -> va);
                        if(mIt == replacingMap.end()) {
                            cout<<"B.1"<<endl;
                            cout<<"matching: "<<(*eIt1) -> vb -> ID<<" to "<<replacingMap[(*eIt2) -> va] -> ID<<endl;
                            replacingMap[(*eIt1) -> vb] = replacingMap[(*eIt2) -> va];
                        } else {
                            cout<<"B.2"<<endl;
                            cout<<"matching: "<<replacingMap[(*eIt2) -> va] -> ID<<" to "<<replacingMap[(*eIt1) -> vb] -> ID<<endl;
                            cout<<"matching: "<<(*eIt2) -> va -> ID<<" to "<<replacingMap[(*eIt1) -> vb] -> ID<<endl;
                            replacingMap[replacingMap[(*eIt2) -> va]] = replacingMap[(*eIt1) -> vb];
                            replacingMap[(*eIt2) -> va] = replacingMap[(*eIt1) -> vb];
                        }
                    }
                }
            } else if(vertexMatch((*eIt1) -> va, (*eIt2) -> va)
             && vertexMatch((*eIt1) -> vb, (*eIt2) -> vb)) {
                cout<<"Hey, I find a mobius replacing"<<endl;
                mIt = replacingMap.find((*eIt2) -> va);
                if(mIt == replacingMap.end()) {
                    mIt = replacingMap.find((*eIt1) -> va);
                    if(mIt == replacingMap.end()) {
                        replacingMap[(*eIt2) -> va] = (*eIt1) -> va;
                    } else {
                        replacingMap[(*eIt2) -> va] = replacingMap[(*eIt1) -> va];
                    }
                } else {
                    if(replacingMap[(*eIt2) -> va] != (*eIt1) -> va) {
                        mIt = replacingMap.find((*eIt1) -> va);
                        if(mIt == replacingMap.end()) {
                            replacingMap[(*eIt1) -> va] = replacingMap[(*eIt2) -> va];
                        } else {
                            replacingMap[replacingMap[(*eIt2) -> va]] = replacingMap[(*eIt1) -> va];
                            replacingMap[(*eIt2) -> va] = replacingMap[(*eIt1) -> va];
                        }
                    }
                }
                mIt = replacingMap.find((*eIt2) -> vb);
                if(mIt == replacingMap.end()) {
                    mIt = replacingMap.find((*eIt1) -> vb);
                    if(mIt == replacingMap.end()) {
                        replacingMap[(*eIt2) -> vb] = (*eIt1) -> vb;
                    } else {
                        replacingMap[(*eIt2) -> vb] = replacingMap[(*eIt1) -> vb];
                    }
                } else {
                    if(replacingMap[(*eIt2) -> vb] != (*eIt1) -> vb) {
                        mIt = replacingMap.find((*eIt1) -> vb);
                        if(mIt == replacingMap.end()) {
                            replacingMap[(*eIt1) -> vb] = replacingMap[(*eIt2) -> vb];
                        } else {
                            replacingMap[replacingMap[(*eIt2) -> vb]] = replacingMap[(*eIt1) -> vb];
                            replacingMap[(*eIt2) -> vb] = replacingMap[(*eIt1) -> vb];
                        }
                    }
                }
            }
        }
    }
    cout<<"I am here!"<<endl;
    Mesh mergedMesh;
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    unordered_map<Vertex*, Vertex*> mergeMap;
    unordered_map<Vertex*, Vertex*> tempMap;
    for(vIt = mesh1.vertList.begin(); vIt < mesh1.vertList.end(); vIt++) {
        mIt = replacingMap.find(*vIt);
        if(mIt == replacingMap.end()) {
            Vertex * v = new Vertex;
            v -> position = (*vIt) -> position;
            v -> ID = mergedMesh.vertList.size();
            mergeMap[*vIt] = v;
            mergedMesh.vertList.push_back(v);
        }
    }
    for(vIt = mesh1.vertList.begin(); vIt < mesh1.vertList.end(); vIt++) {
        mIt = replacingMap.find(*vIt);
        if(mIt != replacingMap.end()) {
            if(mergeMap[replacingMap[*vIt]] == NULL) {
                cout<<"hmm..."<<endl;
            }
            mergeMap[*vIt] = mergeMap[replacingMap[*vIt]];
        }
    }
    cout<<"And here!"<<endl;
    for(vIt = mesh2.vertList.begin(); vIt < mesh2.vertList.end(); vIt++) {
        mIt = replacingMap.find(*vIt);
        if(mIt == replacingMap.end()) {
            Vertex * v = new Vertex;
            v -> position = (*vIt) -> position;
            v -> ID = mergedMesh.vertList.size();
            mergeMap[*vIt] = v;
            mergedMesh.vertList.push_back(v);
        }
    }
    for(vIt = mesh2.vertList.begin(); vIt < mesh2.vertList.end(); vIt++) {
        mIt = replacingMap.find(*vIt);
        if(mIt != replacingMap.end()) {
            if(replacingMap[*vIt] == NULL) {
                cout<<"hmm..."<<endl;
            }
            mergeMap[*vIt] = mergeMap[replacingMap[*vIt]];
        }
    }
    cout<<"And and here?"<<endl;
    for(fIt = mesh1.faceList.begin(); fIt < mesh1.faceList.end(); fIt++) {
        Edge * firstEdge = (*fIt) -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
        do{
            if(currEdge -> fa == (*fIt)) {
                if(mergeMap[currEdge -> vb] == NULL) {
                    cout<<"hmm..."<<endl;
                }
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
    cout<<"And and here??"<<endl;
    for(fIt = mesh2.faceList.begin(); fIt < mesh2.faceList.end(); fIt++) {
        Edge * firstEdge = (*fIt) -> oneEdge;
        Edge * currEdge = firstEdge;
        Edge * nextEdge;
        vector<Vertex*> vertices;
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
    cout<<"And and here..."<<endl;
    return mergedMesh;
}

// Merge any possible boundary edges that are close for multiple meshes.
// @param meshes. The list of meshes to be merged.
// All facets and vertices of mesh2 will be added to mesh1.
// Return a new mesh that contains the merged mesh.
Mesh merge(vector<Mesh*> &meshes) {
    unordered_map<Vertex*, Vertex*> replacingMap;
    Mesh mergedMesh;
    return mergedMesh;
}

// Merge any possible boundary edges that are close for one mesh.
// @param mesh, the mesh of self-merege
Mesh merge(Mesh & mesh) {
    unordered_map<Vertex*, Vertex*> replacingMap;
    unordered_map<Vertex*, Vertex*>::iterator mIt;
    vector<Edge*> boundaryEdgeList = mesh.boundaryEdgeList();
    //cout<<boundaryEdgeList.size()<<endl;
    vector<Edge*>::iterator eIt1;
    vector<Edge*>::iterator eIt2;
    for(eIt1 = boundaryEdgeList.begin(); eIt1 < boundaryEdgeList.end(); eIt1 ++) {
        for(eIt2 = eIt1 + 1; eIt2 < boundaryEdgeList.end(); eIt2 ++) {
            if(vertexMatch((*eIt1) -> va, (*eIt2) -> vb) 
                && vertexMatch((*eIt1) -> vb, (*eIt2) -> va)) {
                //cout<<"Hey, I find a normal replacing"<<endl;
                replacingMap[(*eIt2) -> vb] = (*eIt1) -> va;
                replacingMap[(*eIt2) -> va] = (*eIt1) -> vb;
            } else if(vertexMatch((*eIt1) -> va, (*eIt2) -> va)
             && vertexMatch((*eIt1) -> vb, (*eIt2) -> vb)) {
                //cout<<"Hey, I find a mobius replacing"<<endl;
                replacingMap[(*eIt2) -> va] = (*eIt1) -> va;
                replacingMap[(*eIt2) -> vb] = (*eIt1) -> vb;
            }
        }
    }
    Mesh mergedMesh;
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    unordered_map<Vertex*, Vertex*> mergeMap;
    for(vIt = mesh.vertList.begin(); vIt < mesh.vertList.end(); vIt ++) {
        mIt = replacingMap.find(*vIt);
        if(mIt == replacingMap.end()) {
            Vertex * v = new Vertex;
            v -> position = (*vIt) -> position;
            v -> ID = mergedMesh.vertList.size();
            mergeMap[*vIt] = v;
            //cout<<"mergeMap[v"<<(*vIt) -> ID<<"] = v"<<v -> ID<<endl;
            mergedMesh.vertList.push_back(v);
        } else {
            mergeMap[*vIt] = mergeMap[replacingMap[*vIt]];
            //cout<<"mergeMap[v"<<(*vIt) -> ID<<"] = v"<<mergeMap[replacingMap[*vIt]] -> ID<<endl;
        }
    }
    for(fIt = mesh.faceList.begin(); fIt < mesh.faceList.end(); fIt ++) {
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

#endif // __MESH_H__