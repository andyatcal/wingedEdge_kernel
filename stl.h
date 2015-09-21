// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __STL_H__
#define __STL_H__

#include <glm/glm.hpp>
#include <vector>
#include <iomanip>

using namespace glm;
using namespace std;

// Forward declarations
class Vertex;
class Face;

//////////////////////////////////////////////////////////////////////
// STL Class -- Input and Output with STL Format Files.
//
class STL{
public:
    STL(){};
    void STLOutput(vector<Mesh> &meshes, string outputSTL);
};

void STL::STLOutput(vector<Mesh> &meshes, string outputSTL){
    vector<Mesh>::iterator mIt;
    vector<Face*>::iterator faceIt;
    ofstream file(outputSTL);
    if (!file.is_open()) {
        cout <<"COULD NOT OPEN THE FILE.\n";
    } else {
        file << "solid\n";
        for(mIt = meshes.begin(); mIt < meshes.end(); mIt++) {
            Mesh & currMesh = (*mIt);
            vector<Face*>::iterator fIt;
            for(fIt = currMesh.faceList.begin(); fIt < currMesh.faceList.end(); fIt++) {
                Face * currFace = (*fIt);
                Edge * firstEdge = currFace -> oneEdge;
                Edge * currEdge;
                if(firstEdge == NULL) {
                    cout<<"ERROR: This face does not have a sideEdge."<<endl;
                    exit(0);
                }
                Vertex * v0, * v1, * v2;
                if(currFace == firstEdge -> fa) {
                    v0 = firstEdge -> va;
                    currEdge = firstEdge -> nextVbFa;
                } else {
                    if(firstEdge -> mobius) {
                        v0 = firstEdge -> va;
                        currEdge = firstEdge -> nextVbFb;
                    } else {
                        v0 = firstEdge -> vb;
                        currEdge = firstEdge -> nextVaFb;
                    }
                }
                vec3 p0 = v0 -> position;
                if(currEdge == NULL) {
                    cout<<"ERROR: This face contains only one edge and can not be drawn."<<endl;
                }
                do {
                    Edge * nextEdge;
                    if(currFace == currEdge -> fa) {
                        v1 = currEdge -> va;
                        v2 = currEdge -> vb;
                        nextEdge = currEdge -> nextVbFa;
                    } else {
                        if(currEdge -> mobius) {
                            v1 = currEdge -> va;
                            v2 = currEdge -> vb;
                            nextEdge = currEdge -> nextVbFb;
                        } else {
                            v1 = currEdge -> vb;
                            v2 = currEdge -> va;
                            nextEdge = currEdge -> nextVaFb;
                        }
                    }
                    if(v2 != v0) {
                        vec3 faceNormal = getTriFaceNormal(v0, v1, v2);
                        file << "  facet normal "<<faceNormal[0]<<" "<<faceNormal[1]<<" "<<faceNormal[2]<<"\n";
                        file << "    outer loop\n";
                        vec3 p1 = v1 -> position;
                        vec3 p2 = v2 -> position;
                        file << "      vertex " << p0[0] << " "<< p0[1] << " " << p0[2]<<"\n";
                        file << "      vertex " << p1[0] << " "<< p1[1] << " " << p1[2]<<"\n";
                        file << "      vertex " << p2[0] << " "<< p2[1] << " " << p2[2]<<"\n";
                        file << "    endloop\n";
                        file << "  endfacet\n";
                    }
                    currEdge = nextEdge;
                } while (currEdge != firstEdge);
            }
        }
        file << "endsolid\n";
    }
}
#endif// __STL_H__