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
#include <set>
#include <queue>

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
    unordered_map<Vertex*, set<Vertex*> > map;
    unordered_map<Vertex*, set<Vertex*> > rmap;
    unordered_map<Vertex*, Vertex*> replacingMap;
    unordered_map<Vertex*, set<Vertex*> >::iterator mIt;
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
                //cout<<"Hey, I find a normal replacing"<<endl;
                //cout<<(*eIt1) -> va -> ID<<" matching with "<<(*eIt2) -> vb -> ID<<endl;
                //cout<<(*eIt1) -> vb -> ID<<" matching with "<<(*eIt2) -> va -> ID<<endl;
                mIt = map.find((*eIt1) -> va);
                if(mIt == map.end()) {
                    set<Vertex*> newSet;
                    map[(*eIt1) -> va] = newSet;
                }
                map[(*eIt1) -> va].insert((*eIt2) -> vb);
                mIt = map.find((*eIt1) -> vb);
                if(mIt == map.end()) {
                    set<Vertex*> newSet;
                    map[(*eIt1) -> vb] = newSet;
                }
                map[(*eIt1) -> vb].insert((*eIt2) -> va);
                mIt = rmap.find((*eIt2) -> va);
                if(mIt == rmap.end()) {
                    set<Vertex*> newSet;
                    rmap[(*eIt2) -> va] = newSet;
                }
                rmap[(*eIt2) -> va].insert((*eIt1) -> vb);
                mIt = rmap.find((*eIt2) -> vb);
                if(mIt == rmap.end()) {
                    set<Vertex*> newSet;
                    rmap[(*eIt2) -> vb] = newSet;
                }
                rmap[(*eIt2) -> vb].insert((*eIt1) -> va);
            } else if(vertexMatch((*eIt1) -> va, (*eIt2) -> va)
             && vertexMatch((*eIt1) -> vb, (*eIt2) -> vb)) {
                //cout<<"Hey, I find a mobius replacing"<<endl;
                //cout<<(*eIt1) -> va -> ID<<" matching with "<<(*eIt2) -> va -> ID<<endl;
                //cout<<(*eIt1) -> vb -> ID<<" matching with "<<(*eIt2) -> vb -> ID<<endl;
                mIt = map.find((*eIt1) -> va);
                if(mIt == map.end()) {
                    set<Vertex*> newSet;
                    map[(*eIt1) -> va] = newSet;
                }
                map[(*eIt1) -> va].insert((*eIt2) -> va);
                mIt = map.find((*eIt1) -> vb);
                if(mIt == map.end()) {
                    set<Vertex*> newSet;
                    map[(*eIt1) -> vb] = newSet;
                }
                map[(*eIt1) -> vb].insert((*eIt2) -> vb);
                mIt = rmap.find((*eIt2) -> va);
                if(mIt == rmap.end()) {
                    set<Vertex*> newSet;
                    rmap[(*eIt2) -> va] = newSet;
                }
                rmap[(*eIt2) -> va].insert((*eIt1) -> va);
                mIt = rmap.find((*eIt2) -> vb);
                if(mIt == rmap.end()) {
                    set<Vertex*> newSet;
                    rmap[(*eIt2) -> vb] = newSet;
                }
                rmap[(*eIt2) -> vb].insert((*eIt1) -> vb);
            }
        }
    }

    int counter = 0;
    set<Vertex*>::iterator sIt;
    set<Vertex*>::iterator siIt;
    set<Vertex*> vertices;
    set<Vertex*> verticesInner;
    Vertex * key;
    unordered_map<Vertex*, set<Vertex*> > tempMap1;
    while(map.size() != 0) {
        mIt = map.begin();
        queue<Vertex*> currQueue;
        key = mIt -> first;
        set<Vertex*> values;
        //cout<<"Key: "<< mIt -> first -> ID<<" mapped to ";
        currQueue.push(mIt ->first);
        while(!currQueue.empty()) {
            Vertex * curr = currQueue.front();
            currQueue.pop();
            mIt = map.find(curr);
            if(mIt != map.end()) {
                vertices = mIt -> second;
                values.insert(curr);
                map.erase(mIt);
                for(sIt = vertices.begin(); sIt != vertices.end(); sIt++) {
                    mIt = rmap.find(*sIt);
                    verticesInner = mIt -> second;
                    values.insert(mIt -> first);
                    for(siIt = verticesInner.begin(); siIt != verticesInner.end(); siIt++) {
                        mIt = map.find(*siIt);
                        if(mIt != map.end()) {
                            currQueue.push(mIt -> first);
                        }
                    }
                }
            }
            tempMap1[key] = values;
        }
        //for(sIt = vertices.begin(); sIt != vertices.end(); sIt++) {
        //    cout<<(*sIt) -> ID<<" ";
        //}
        //cout<<endl;
    }
    // Reverse the map
    for(mIt = tempMap1.begin(); mIt != tempMap1.end(); mIt++) {
        for(sIt = (mIt -> second).begin(); sIt != (mIt -> second).end(); sIt++) {
            if((*sIt) != (mIt -> first)) {
                //cout<<"Mapping "<<(*sIt) -> ID<<" to "<<mIt -> first -> ID<<endl;
                replacingMap[*sIt] = mIt -> first;
            }
        }
    }

    // Create new vertices for the merged mesh and generate links.
    //cout<<"I am here!"<<endl;
    Mesh mergedMesh;
    vector<Vertex*>::iterator vIt;
    vector<Face*>::iterator fIt;
    unordered_map<Vertex*, Vertex*>::iterator merIt;
    unordered_map<Vertex*, Vertex*> mergeMap;
    unordered_map<Vertex*, Vertex*> tempMap;
    for(vIt = mesh1.vertList.begin(); vIt < mesh1.vertList.end(); vIt++) {
        merIt = replacingMap.find(*vIt);
        if(merIt == replacingMap.end()) {
            Vertex * v = new Vertex;
            v -> position = (*vIt) -> position;
            v -> ID = mergedMesh.vertList.size();
            mergeMap[*vIt] = v;
            mergedMesh.vertList.push_back(v);
        }
    }
    for(vIt = mesh1.vertList.begin(); vIt < mesh1.vertList.end(); vIt++) {
        merIt = replacingMap.find(*vIt);
        if(merIt != replacingMap.end()) {
            mergeMap[*vIt] = mergeMap[replacingMap[*vIt]];
        }
    }
    //cout<<"And here!"<<endl;
    for(vIt = mesh2.vertList.begin(); vIt < mesh2.vertList.end(); vIt++) {
        merIt = replacingMap.find(*vIt);
        if(merIt == replacingMap.end()) {
            Vertex * v = new Vertex;
            v -> position = (*vIt) -> position;
            v -> ID = mergedMesh.vertList.size();
            mergeMap[*vIt] = v;
            mergedMesh.vertList.push_back(v);
        }
    }
    for(vIt = mesh2.vertList.begin(); vIt < mesh2.vertList.end(); vIt++) {
        merIt = replacingMap.find(*vIt);
        if(merIt != replacingMap.end()) {
            mergeMap[*vIt] = mergeMap[replacingMap[*vIt]];
        }
    }
    for(fIt = mesh1.faceList.begin(); fIt < mesh1.faceList.end(); fIt++) {
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
    return mergedMesh;
}

// Merge any possible boundary edges that are close for multiple meshes.
// @param meshes. The list of meshes to be merged.
// All facets and vertices of mesh2 will be added to mesh1.
// Return a new mesh that contains the merged mesh.
Mesh merge(vector<Mesh> &meshes) {
    Mesh mergedMesh;
    vector<Mesh>::iterator meshIt;
    for(meshIt = meshes.begin(); meshIt < meshes.end(); meshIt++) {
        mergedMesh = merge(mergedMesh, *meshIt);
    }
    return mergedMesh;
}

#endif // __MESH_H__