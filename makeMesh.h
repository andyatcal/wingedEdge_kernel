
// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __MAKEMESH_H__
#define __MAKEMESH_H__

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// MakeMesh Class -- Create Initial Meshes.
void makeSquare(Mesh &mesh) {
    //make new mesh
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;

    v1 -> position = vec3(1, 1, 0);
    v2 -> position = vec3(1, -1, 0);
    v3 -> position = vec3(-1, -1, 0);
    v4 -> position = vec3(-1, 1, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addQuadFace(v1, v2, v3, v4);
    mesh.buildBoundary();
}

Mesh mergeTwoMeshes1() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;
    Vertex * v9 = new Vertex;
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;

    v1 -> position = vec3(1, 1, 0);
    v2 -> position = vec3(-1, 1, 0);
    v3 -> position = vec3(-1, 0, 0);
    v4 -> position = vec3(1, 0, 0);
    v5 -> position = vec3(1, 0, 0);
    v6 -> position = vec3(-1, 0, 0);
    v7 -> position = vec3(-1, -1, 0);
    v8 -> position = vec3(1, -1, 0);
    v9 -> position = vec3(0.5, 0, 0);
    v10 -> position = vec3(0.5, 0, 0);
    v11 -> position = vec3(-0.5, 0, 0);
    v12 -> position = vec3(-0.5, 0, 0);
    
    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;
    v9 -> ID = 8;
    v10 -> ID = 9;
    v11 -> ID = 10;
    v12 -> ID = 11;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);
    mesh1.addVertex(v4);
    mesh2.addVertex(v5);
    mesh2.addVertex(v6);
    mesh2.addVertex(v7);
    mesh2.addVertex(v8);
    mesh1.addVertex(v9);
    mesh2.addVertex(v10);
    mesh1.addVertex(v11);
    mesh2.addVertex(v12);
    vector<Vertex*> topFace;
    vector<Vertex*> bottomFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);
    topFace.push_back(v9);
    topFace.push_back(v11);
    topFace.push_back(v4);
    bottomFace.push_back(v5);
    bottomFace.push_back(v12);
    bottomFace.push_back(v10);
    bottomFace.push_back(v6);
    bottomFace.push_back(v7);
    bottomFace.push_back(v8);

    mesh1.addPolygonFace(topFace, true);
    mesh2.addPolygonFace(bottomFace, true);
    mesh1.buildBoundary();
    mesh2.buildBoundary();

    //mesh1.addQuadFace(v1, v2, v3, v4);
    //mesh2.addQuadFace(v5, v6, v7, v8);
    return merge(mesh1, mesh2);
    //return mesh1;
}

Mesh mergeTwoMeshes2() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;

    v1 -> position = vec3(1, 1, 0);
    v2 -> position = vec3(-1, 1, 0);
    v3 -> position = vec3(-1, 0, 0);
    v4 -> position = vec3(1, 0, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);
    mesh1.addVertex(v4);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);
    topFace.push_back(v4);

    mesh1.addPolygonFace(topFace, true);
    mesh1.buildBoundary();

    mesh2 = meshCopy(mesh1);
    //mat4 rot = krotate(vec3(0, 1, 1), PI);
    mat4 translate = ktranslate(vec3(0, -1, 0));
    transform(mesh2, translate);
    //transform(mesh2, rot);
    Mesh merged = merge(mesh1, mesh2);
    return merged;
}

Mesh noMergeTwoMeshes2() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;
    v1 -> position = vec3(1, 1, 0);
    v2 -> position = vec3(-1, 1, 0);
    v3 -> position = vec3(-1, 0, 0);
    v4 -> position = vec3(1, 0, 0);
    v5 -> position = vec3(1, 0, 0);
    v6 -> position = vec3(-1, 0, 0);
    v7 -> position = vec3(-1, -1, 0);
    v8 -> position = vec3(1, -1, 0);
    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);
    mesh1.addVertex(v4);
    mesh1.addVertex(v5);
    mesh1.addVertex(v6);
    mesh1.addVertex(v7);
    mesh1.addVertex(v8);
    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);
    topFace.push_back(v4);

    mesh1.addPolygonFace(topFace, true);
    topFace.clear();
    topFace.push_back(v5);
    topFace.push_back(v6);
    topFace.push_back(v7);
    topFace.push_back(v8);

    mesh1.addPolygonFace(topFace, true);
    mesh1.buildBoundary();

    //mesh2 = meshCopy(mesh1);
    //mat4 rot = krotate(vec3(0, 1, 1), PI);
    //mat4 translate = ktranslate(vec3(0, -1, 0));
    //transform(mesh2, translate);
    //transform(mesh2, rot);
    //Mesh merged = merge(mesh1, mesh2);
    //return merged;
    return mesh1;
}
Mesh mergeTwoMeshes3() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;


    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);
    mesh1.buildBoundary();

    mesh2 = meshCopy(mesh1);
    mat4 rot = krotate(vec3(0, 0, 1), PI/3);
    transform(mesh2, rot);

    Mesh merged = merge(mesh1, mesh2);
    return merged;
}

Mesh mergeTwoMeshes8() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Mesh mesh3;

    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;

    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);

    mat4 rot1 = krotate(vec3(0,0,1), PI / 3);
    mat4 rot2 = krotate(vec3(0,0,1), PI * 2 / 3);
    mat4 rot3 = krotate(vec3(0,0,1), PI);
    mesh2 = meshCopy(mesh1);
    mesh3 = meshCopy(mesh1);
    mat4 mirror = kmirror(vec4(0, 1, 0, 0));
    transform(mesh2, rot1);
    transform(mesh3, mirror);
    Mesh merged = merge(mesh1, mesh2);
    //merged = merge(merged, mesh3);
    Mesh mesh5 = meshCopy(mesh3);
    transform(mesh5, rot3);
    Mesh merged2 = merge(mesh3, mesh5);
    Mesh mesh4 = meshCopy(merged);
    transform(mesh4, rot3);
    merged = merge(merged, mesh4);
    merged = merge(merged, merged2);
    return merged;
}

Mesh mergeTwoMeshes4() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Mesh mesh3;

    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;

    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);

    mat4 rot1 = krotate(vec3(0,0,1), PI / 3);
    mat4 rot2 = krotate(vec3(0,0,1), PI * 2 / 3);
    mat4 rot3 = krotate(vec3(0,0,1), PI);
    mesh2 = meshCopy(mesh1);
    mesh3 = meshCopy(mesh1);
    mat4 mirror = kmirror(vec4(0, 1, 0, 0));
    transform(mesh2, rot1);
    transform(mesh3, mirror);
    vector<Mesh> meshes;
    meshes.push_back(mesh1);
    meshes.push_back(mesh2);
    meshes.push_back(mesh3);
    Mesh merged = merge(meshes);
    //merged = merge(merged, mesh3);
    //Mesh mesh5 = meshCopy(mesh3);
    //transform(mesh5, rot3);
    //Mesh merged2 = merge(mesh3, mesh5);
    //Mesh mesh4 = meshCopy(merged);
    //transform(mesh4, rot3);
    //merged = merge(merged, mesh4);
    //merged = merge(merged, merged2);
    return merged;
}

Mesh mergeTwoMeshes7() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Mesh mesh3;

    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;

    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);

    mat4 rot1 = krotate(vec3(0,0,1), PI * 2 / 3);
    mat4 rot2 = krotate(vec3(0,0,1), - PI * 2 / 3);
    mat4 translation = ktranslate(vec3(-0.5, 0.866, 0));
    mesh2 = meshCopy(mesh1);
    mesh3 = meshCopy(mesh1);

    transform(mesh2, rot1);
    transform(mesh3, translation);
    
    Mesh merged = merge(mesh1, mesh2);
    merged = merge(merged, mesh3);

    mat4 rot3 = krotate(vec3(0,0,1), PI /3);
    Mesh mesh4 = meshCopy(mesh1);
    transform(mesh4, rot3);
    merged = merge(merged, mesh4);
    return merged;
}

Mesh mergeTwoMeshes6() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;

    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;
    Vertex * v9 = new Vertex;
    Vertex * v10 = new Vertex;
    Vertex * v11 = new Vertex;
    Vertex * v12 = new Vertex;
    Vertex * v13 = new Vertex;
    Vertex * v14 = new Vertex;
    Vertex * v15 = new Vertex;
    Vertex * v16 = new Vertex;
    Vertex * v17 = new Vertex;
    Vertex * v18 = new Vertex;

    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);
    
    mesh1.addVertex(v4);
    mesh1.addVertex(v5);
    mesh1.addVertex(v6);

    mesh1.addVertex(v7);
    mesh1.addVertex(v8);
    mesh1.addVertex(v9);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);

    v4 -> position = vec3(0, 0, 0);
    v5 -> position = vec3(-0.5, 0.866, 0);
    v6 -> position = vec3(-1, 0, 0);

    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;

    topFace.clear();
    topFace.push_back(v4);
    topFace.push_back(v5);
    topFace.push_back(v6);

    mesh1.addPolygonFace(topFace, true);

    v7 -> position = vec3(0, 0, 0);
    v8 -> position = vec3(-0.5, -0.866, 0);
    v9 -> position = vec3(0.5, -0.866, 0);

    v7 -> ID = 6;
    v8 -> ID = 7;
    v9 -> ID = 8;
    
    topFace.clear();
    topFace.push_back(v7);
    topFace.push_back(v8);
    topFace.push_back(v9);

    mesh1.addPolygonFace(topFace, true);
    mesh1.buildBoundary();

    v10 -> position = vec3(0, 0, 0);
    v11 -> position = vec3(0.5, 0.866, 0);
    v12 -> position = vec3(-0.5, 0.866, 0);

    v10 -> ID = 9;
    v11 -> ID = 10;
    v12 -> ID = 11;
    
    mesh2.addVertex(v10);
    mesh2.addVertex(v11);
    mesh2.addVertex(v12);

    topFace.clear();
    topFace.push_back(v10);
    topFace.push_back(v11);
    topFace.push_back(v12);

    mesh2.addPolygonFace(topFace, true);

    v13 -> position = vec3(0, 0, 0);
    v14 -> position = vec3(-1, 0, 0);
    v15 -> position = vec3(-0.5, -0.866, 0);

    v13 -> ID = 12;
    v14 -> ID = 13;
    v15 -> ID = 14;
    
    mesh2.addVertex(v13);
    mesh2.addVertex(v14);
    mesh2.addVertex(v15);

    topFace.clear();
    topFace.push_back(v13);
    topFace.push_back(v14);
    topFace.push_back(v15);

    mesh2.addPolygonFace(topFace, true);

    v16 -> position = vec3(0, 0, 0);
    v17 -> position = vec3(0.5, -0.866, 0);
    v18 -> position = vec3(1, 0, 0);

    v16 -> ID = 15;
    v17 -> ID = 16;
    v18 -> ID = 17;
    
    mesh2.addVertex(v16);
    mesh2.addVertex(v17);
    mesh2.addVertex(v18);

    topFace.clear();
    topFace.push_back(v16);
    topFace.push_back(v17);
    topFace.push_back(v18);

    mesh2.addPolygonFace(topFace, true);
    mesh2.buildBoundary();
    
    Mesh merged = merge(mesh1, mesh2);
    return merged;
}

Mesh mergeTwoMeshes5() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;


    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    
    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);
    mesh1.buildBoundary();

    mesh2 = meshCopy(mesh1);
    mat4 rot = krotate(vec3(0, 0, 1), PI/3);
    transform(mesh2, rot);

    Mesh merged = merge(mesh1, mesh2);
    mat4 rot2 = krotate(vec3(0, 0, 1), PI * 2 / 3);
    Mesh mesh3 = meshCopy(merged);
    Mesh mesh4 = meshCopy(merged);
    transform(mesh3, rot2);
    transform(mesh4, rot2);
    transform(mesh4, rot2);
    merged = merge(mesh3, merged);
    merged = merge(mesh4, merged);
    return merged;
}

Mesh mirrorTest() {
    //make new mesh
    Mesh mesh1;
    Mesh mesh2;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;


    v1 -> position = vec3(0, 0, 0);
    v2 -> position = vec3(1, 0, 0);
    v3 -> position = vec3(0.5, 0.866, 0);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;

    mesh1.addVertex(v1);
    mesh1.addVertex(v2);
    mesh1.addVertex(v3);

    vector<Vertex*> topFace;
    topFace.push_back(v1);
    topFace.push_back(v2);
    topFace.push_back(v3);

    mesh1.addPolygonFace(topFace, true);
    mesh1.buildBoundary();

    //Set the mirror plane here.

    vec4 mirror_plane = vec4(0, 1, 1, 0);
    mat4 mirror = kmirror(mirror_plane);
    // Interesting, the glm implements the translation differently.
    //mat4 mirror = translate(vec3(-2, 0, 0));
    //cout<<mirror[0][0]<<" "<<mirror[0][1]<<" "<<mirror[0][2]<<" "<<mirror[0][3]<<endl;
    //cout<<mirror[1][0]<<" "<<mirror[1][1]<<" "<<mirror[1][2]<<" "<<mirror[1][3]<<endl;
    //cout<<mirror[2][0]<<" "<<mirror[2][1]<<" "<<mirror[2][2]<<" "<<mirror[2][3]<<endl;
    //cout<<mirror[3][0]<<" "<<mirror[3][1]<<" "<<mirror[3][2]<<" "<<mirror[3][3]<<endl;
    mesh2 = meshCopy(mesh1);
    transform(mesh2, mirror);

    Mesh merged = merge(mesh1, mesh2);
    return merged;
}

void makePyramid(Mesh &mesh){
    //make new mesh
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;

    v1 -> position = vec3(0, 0, 1);
    v2 -> position = vec3(1, 1, -1);
    v3 -> position = vec3(1, -1, -1);
    v4 -> position = vec3(-1, -1, -1);
    v5 -> position = vec3(-1, 1, -1);

    v1 -> ID = 1;
    v2 -> ID = 2;
    v3 -> ID = 3;
    v4 -> ID = 4;
    v5 -> ID = 5;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);

    vector<Vertex*> bottomFace;
    vector<Vertex*> bottomFaceR;
    bottomFace.push_back(v3);
    bottomFace.push_back(v2);
    bottomFace.push_back(v5);
    bottomFace.push_back(v4);

    bottomFaceR.push_back(v4);
    bottomFaceR.push_back(v5);
    bottomFaceR.push_back(v2);
    bottomFaceR.push_back(v3);

    //bottomFace
    mesh.addPolygonFace(bottomFace);
    //topfrontFace
    mesh.addTriFace(v1, v2, v3);
    //topbackFace
    mesh.addTriFace(v1, v3, v4);
    //topleftFace
    mesh.addTriFace(v1, v4, v5);
    //toprightFace
    mesh.addTriFace(v1, v5, v2);
    //cout<<mesh.faceList.size()<<endl;
    //cout<<mesh.vertList.size()<<endl;
    //cout<<mesh.edgeTable.size()<<endl;
    mesh.buildBoundary();
}

void makeCube(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;

    v1 -> position = vec3(1, 1, 1);
    v2 -> position = vec3(1, -1, 1);
    v3 -> position = vec3(-1, -1, 1);
    v4 -> position = vec3(-1, 1, 1);
    v5 -> position = vec3(1, 1, -1);
    v6 -> position = vec3(1, -1, -1);
    v7 -> position = vec3(-1, -1, -1);
    v8 -> position = vec3(-1, 1, -1);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);
 
    //topFace
    mesh.addQuadFace(v1, v2, v3, v4);
    //bottomFace
    mesh.addQuadFace(v6, v5, v8, v7);
    //leftFace
    mesh.addQuadFace(v3, v2, v6, v7);
    //rightFace
    mesh.addQuadFace(v1, v4, v8, v5);
    //frontFace
    mesh.addQuadFace(v2, v1, v5, v6);
    //baceFace
    mesh.addQuadFace(v4, v3, v7, v8);
    mesh.buildBoundary();
}

void makeCube(Mesh &mesh, float x, float y, float z) {
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;

    v1 -> position = vec3(x, y, z);
    v2 -> position = vec3(x, -y, z);
    v3 -> position = vec3(-x, -y, z);
    v4 -> position = vec3(-x, y, z);
    v5 -> position = vec3(x, y, -z);
    v6 -> position = vec3(x, -y, -z);
    v7 -> position = vec3(-x, -y, -z);
    v8 -> position = vec3(-x, y, -z);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);
 
    //topFace
    mesh.addQuadFace(v1, v2, v3, v4);
    //bottomFace
    mesh.addQuadFace(v6, v5, v8, v7);
    //leftFace
    mesh.addQuadFace(v3, v2, v6, v7);
    //rightFace
    mesh.addQuadFace(v1, v4, v8, v5);
    //frontFace
    mesh.addQuadFace(v2, v1, v5, v6);
    //baceFace
    mesh.addQuadFace(v4, v3, v7, v8);
    mesh.buildBoundary();
}

void makeOpenCube(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;
 
    v1 -> position = vec3(1, 1, 1);
    v2 -> position = vec3(1, -1, 1);
    v3 -> position = vec3(-1, -1, 1);
    v4 -> position = vec3(-1, 1, 1);
    v5 -> position = vec3(1, 1, -1);
    v6 -> position = vec3(1, -1, -1);
    v7 -> position = vec3(-1, -1, -1);
    v8 -> position = vec3(-1, 1, -1);

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);

    //without the topFace
    //mesh.addQuadFace(v1, v2, v3, v4);
    //bottomFace
    mesh.addQuadFace(v6, v5, v8, v7);
    //leftFace
    mesh.addQuadFace(v3, v2, v6, v7);
    //rightFace
    mesh.addQuadFace(v1, v4, v8, v5);
    //frontFace
    mesh.addQuadFace(v2, v1, v5, v6);
    //baceFace
    mesh.addQuadFace(v4, v3, v7, v8);
    mesh.buildBoundary();
}

void makeRing(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;
 
    v1 -> position = vec3(1, 1, 1);
    v2 -> position = vec3(1, -1, 1);
    v3 -> position = vec3(-1, -1, 1);
    v4 -> position = vec3(-1, 1, 1);
    v5 -> position = vec3(1, 1, -1);
    v6 -> position = vec3(1, -1, -1);
    v7 -> position = vec3(-1, -1, -1);
    v8 -> position = vec3(-1, 1, -1);

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);

    //leftFace
    mesh.addQuadFace(v3, v2, v6, v7);
    //rightFace
    mesh.addQuadFace(v1, v4, v8, v5);
    //frontFace
    mesh.addQuadFace(v2, v1, v5, v6);
    //baceFace
    mesh.addQuadFace(v4, v3, v7, v8);
    mesh.buildBoundary();  
}

void makeSharpCube(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;
 
    v1 -> position = vec3(1, 1, 1);
    v2 -> position = vec3(1, -1, 1);
    v3 -> position = vec3(-1, -1, 1);
    v4 -> position = vec3(-1, 1, 1);
    v5 -> position = vec3(1, 1, -1);
    v6 -> position = vec3(1, -1, -1);
    v7 -> position = vec3(-1, -1, -1);
    v8 -> position = vec3(-1, 1, -1);

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);

    //without the topFace
    mesh.addQuadFace(v1, v2, v3, v4);
    //bottomFace
    mesh.addQuadFace(v6, v5, v8, v7);
    //leftFace
    mesh.addQuadFace(v3, v2, v6, v7);
    //rightFace
    mesh.addQuadFace(v1, v4, v8, v5);
    //frontFace
    mesh.addQuadFace(v2, v1, v5, v6);
    //baceFace
    mesh.addQuadFace(v4, v3, v7, v8);
    
    
    /*
    unordered_map<unsigned long long, Halfedge*>::iterator eIt;
    for(eIt = edgeTable.begin(); eIt != edgeTable.end(); eIt++) {
        if (eIt -> second -> start == v1 || eIt -> second -> end == v1) {
            eIt -> second -> isSharp = true;
        }

        else if(eIt -> second -> start == v2 && eIt -> second -> end == v1) {
            eIt -> second -> isSharp = true;
        } else if (eIt -> second -> start == v2 && eIt -> second -> end == v3) {
            eIt -> second -> isSharp = true;
        } else if(eIt -> second -> start == v3 && eIt -> second -> end == v2) {
            eIt -> second -> isSharp = true;
        } else if (eIt -> second -> start == v3 && eIt -> second -> end == v4) {
            eIt -> second -> isSharp = true;
        } else if(eIt -> second -> start == v4 && eIt -> second -> end == v3) {
            eIt -> second -> isSharp = true;
        } else if (eIt -> second -> start == v1 && eIt -> second -> end == v4) {
            eIt -> second -> isSharp = true;
        } else if(eIt -> second -> start == v4 && eIt -> second -> end == v1) {
            eIt -> second -> isSharp = true;
        }

    }
    */
    mesh.buildBoundary();
}

void makeOctahedron(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
 
    v1 -> position = vec3(0, 0, 1.414);
    v2 -> position = vec3(1, 1, 0);
    v3 -> position = vec3(1, -1, 0);
    v4 -> position = vec3(-1, -1, 0);
    v5 -> position = vec3(-1, 1, 0);
    v6 -> position = vec3(0, 0, -1.414);

    mesh.addTriFace(v1, v2, v3);
    mesh.addTriFace(v1, v3, v4);
    mesh.addTriFace(v1, v4, v5);
    mesh.addTriFace(v1, v5, v2);
    mesh.addTriFace(v6, v3, v2);
    mesh.addTriFace(v6, v4, v3);
    mesh.addTriFace(v6, v5, v4);
    mesh.addTriFace(v6, v2, v5);
}

void makeSharpOctahedron(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
 
    v1 -> position = vec3(0, 0, 1.414);
    v2 -> position = vec3(1, 1, 0);
    v3 -> position = vec3(1, -1, 0);
    v4 -> position = vec3(-1, -1, 0);
    v5 -> position = vec3(-1, 1, 0);
    v6 -> position = vec3(0, 0, -1.414);

    mesh.addTriFace(v1, v2, v3);
    mesh.addTriFace(v1, v3, v4);
    mesh.addTriFace(v1, v4, v5);
    mesh.addTriFace(v1, v5, v2);
    mesh.addTriFace(v6, v3, v2);
    mesh.addTriFace(v6, v4, v3);
    mesh.addTriFace(v6, v5, v4);
    mesh.addTriFace(v6, v2, v5);

    //Should define sharp edge here.
    mesh.buildBoundary();
}

void makeNormalStrip(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);

    v1 -> position = vec3(1.5, 1.5, 0);
    v2 -> position = vec3(1.5, -1.5, 0);
    v3 -> position = vec3(-1.5, -1.5, 0);
    v4 -> position = vec3(-1.5, 1.5, 0);
    v5 -> position = vec3(0.5, 0.5, 0);
    v6 -> position = vec3(0.5, -0.5, 0);
    v7 -> position = vec3(-0.5, -0.5, 0);
    v8 -> position = vec3(-0.5, 0.5, 0);

    mesh.addQuadFace(v1, v5, v6, v2);
    mesh.addQuadFace(v3, v2, v6, v7);
    mesh.addQuadFace(v4, v3, v7, v8);
    mesh.addQuadFace(v1, v4, v8, v5);
    mesh.buildBoundary();
}

void makeMobius(Mesh &mesh){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    //make new mesh
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * v3 = new Vertex;
    Vertex * v4 = new Vertex;
    Vertex * v5 = new Vertex;
    Vertex * v6 = new Vertex;
    Vertex * v7 = new Vertex;
    Vertex * v8 = new Vertex;
    Vertex * v9 = new Vertex;
    Vertex * vX = new Vertex;

    v1 -> position = vec3(1.5, 1.5, 0);
    v2 -> position = vec3(1.5, -1.5, 0);
    v3 -> position = vec3(-1.5, -1.5, 0);
    v4 -> position = vec3(-1.5, 1.5, 0);
    v5 -> position = vec3(0.5, 0.5, 0);
    v6 -> position = vec3(0.5, -0.5, 0);
    v7 -> position = vec3(-0.5, -0.5, 0);
    v8 -> position = vec3(-0.5, 0.5, 0);
    v9 -> position = vec3(1, 0, 0.5);
    vX -> position = vec3(1, 0, -0.5);

    v1 -> ID = 0;
    v2 -> ID = 1;
    v3 -> ID = 2;
    v4 -> ID = 3;
    v5 -> ID = 4;
    v6 -> ID = 5;
    v7 -> ID = 6;
    v8 -> ID = 7;
    v9 -> ID = 8;
    vX -> ID = 9;

    //push on all new verts
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    mesh.addVertex(v4);
    mesh.addVertex(v5);
    mesh.addVertex(v6);
    mesh.addVertex(v7);
    mesh.addVertex(v8);
    mesh.addVertex(v9);
    mesh.addVertex(vX);

    mesh.addQuadFace(v1, v5, vX, v9);
    mesh.addQuadFace(vX, v9, v6, v2);
    mesh.addQuadFace(v3, v2, v6, v7);
    mesh.addQuadFace(v4, v3, v7, v8);
    mesh.addQuadFace(v1, v4, v8, v5);
    mesh.buildBoundary();
}

void makeCircleSweep(Mesh &mesh) {
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    int loop_test = 0;
    int l_slices = 36;
    float total_arc = 360.0;
    float step = total_arc / l_slices;
    vector<vector<Vertex*> > vertices;

    for(float i = 0.0; i <= total_arc; i += step) {
        vector<Vertex*> crossSection;
        float angle = i / 180 * PI;
        vec3 zaxis = vec3(0, 0, 1);
        vec3 xaxis = vec3(1, 0, 0);
        vec3 v1 = vec3(0, 2, 1);
        vec3 v2 = vec3(0, 1, 2);
        vec3 v3 = vec3(0, -1, -2);
        vec3 v4 = vec3(0, -2, -1);
        vec3 trans = vec3(0 , 3, 0);

        // I will twist m * 180 of this sweep.
        int m = 1;

        v1 = rotate(v1, m*angle/2, xaxis);
        v2 = rotate(v2, m*angle/2, xaxis);
        v3 = rotate(v3, m*angle/2, xaxis);
        v4 = rotate(v4, m*angle/2, xaxis);
        
        v1 += trans;
        v2 += trans;
        v3 += trans;
        v4 += trans;

        v1 = rotate(v1, angle, zaxis);
        v2 = rotate(v2, angle, zaxis);
        v3 = rotate(v3, angle, zaxis);
        v4 = rotate(v4, angle, zaxis);

        if(i == total_arc) {
            if(distance(v1, vertices[0][0] -> position) < VERYSMALLVALUE
             && distance(v4, vertices[0][3] -> position) < VERYSMALLVALUE) {
                loop_test = 1;
                //cout<<"This is a normal loop."<<endl;
                break;
            } else if(distance(v1, vertices[0][3] -> position) < VERYSMALLVALUE
             && distance(v4, vertices[0][0] -> position) < VERYSMALLVALUE) {
                //cout<<"This is a moibus loop."<<endl;
                loop_test = 2;
                break;
            }
        }

        Vertex * P1 = new Vertex;
        Vertex * P2 = new Vertex;
        Vertex * P3 = new Vertex;
        Vertex * P4 = new Vertex;

        P1 -> position = v1;
        P2 -> position = v2;
        P3 -> position = v3;
        P4 -> position = v4;
        P1 -> ID = mesh.vertList.size();
        mesh.addVertex(P1);
        P2 -> ID = mesh.vertList.size();
        mesh.addVertex(P2);
        P3 -> ID = mesh.vertList.size();
        mesh.addVertex(P3);
        P4 -> ID = mesh.vertList.size();
        mesh.addVertex(P4);
        crossSection.push_back(P1);
        crossSection.push_back(P2);
        crossSection.push_back(P3);
        crossSection.push_back(P4);
        vertices.push_back(crossSection);
    }
    for(size_t j = 0; j < vertices.size() - 1; j += 1) {
        for(size_t k = 0; k < 3; k += 1){
            Vertex * v1 = vertices[j][k];
            Vertex * v2 = vertices[j + 1][k];
            Vertex * v3 = vertices[j + 1][k + 1];
            Vertex * v4 = vertices[j][k + 1];
            mesh.addQuadFace(v1, v2, v3, v4);
        }
    }
    if(loop_test == 1) { //If it is a loop //allignment test
        size_t j = vertices.size() - 1;
        for(size_t k = 0; k < 3; k += 1){
            Vertex * v1 = vertices[j][k];
            Vertex * v2 = vertices[0][k];
            Vertex * v3 = vertices[0][k + 1];
            Vertex * v4 = vertices[j][k + 1];
            mesh.addQuadFace(v1, v2, v3, v4);
        }        
    }
    if(loop_test == 2) { //If it is a mobius loop
        size_t j = vertices.size() - 1;
        for(size_t k = 0; k < 3; k += 1){
            Vertex * v1 = vertices[j][3 - k - 1];
            Vertex * v2 = vertices[j][3 - k];
            Vertex * v3 = vertices[0][k];
            Vertex * v4 = vertices[0][k + 1];
            mesh.addQuadFace(v1, v2, v3, v4);
        }        
    }
    mesh.buildBoundary();
}

void makeWithSIF(Mesh &mesh, string inputSIF){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    ifstream file(inputSIF);
    if (!file.good()) {
        cout<<"THE PATH OF SIF FILE IS NOT VAILD.";
        exit(1); // exit if file not found
    }
    string nextLine;
    regex anyR(".*(.*).*");
    regex vRegex(".*\(v .*$\).*");
    regex tRegex(".*\(t .*\).*");
    regex lRegex(".*\(loop .*\).*");
    regex shRegex(".*\\(shell.*\).*");
    regex verticesRegex(".*\\(vertices .*\).*");
    int vBeforeMergeCounter = 0;
    int vAfterMergeCounter = 0;
    int IDplusBecauseOfShells = 0;
    vector<vector<int> > boundaries;
    unordered_map<int, int> mapBeforeMergeToAfter;
    int shellNum = 0;
    vector<int> numberOfVerticesInShells;
    while(getline(file, nextLine)){
        nextLine.pop_back();
        if(regex_match(nextLine, vRegex)){
            string temp;
            temp = nextLine.substr(nextLine.find("\("), nextLine.find("\)") - nextLine.find("\("));
            temp = temp.substr(temp.find(" ") + 1);
            float x = stof(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            float y = stof(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            float z = stof(temp);
            Vertex * newVert = new Vertex;
            newVert -> position = vec3(x, y, z) * 1.0f; // Can be modifed here to zoom in.
            newVert -> ID = vAfterMergeCounter;
            vector<Vertex*>::iterator vIt;
            bool alreadyAdded = false;
            for (vIt = mesh.vertList.begin(); vIt < mesh.vertList.end(); vIt ++) {
                if(distance(newVert -> position, (*vIt) -> position) < 0.001 ){
                    //cout << "The distance between vertex "<<newVert -> ID<<" and vertex "<<vIt -> second -> ID<<" is: "<<endl;
                    //cout << newVert -> position - (*vIt) -> position<<endl;
                    alreadyAdded = true;
                    mapBeforeMergeToAfter[vBeforeMergeCounter] = (*vIt) -> ID;
                    break;
                }
            }
            //cout<<newVert -> ID<<"Vertex added"<<endl;
            //cout<<"I am mapping "<<vBeforeMergeCounter<<" to "<<vAfterMergeCounter<<endl;
            //mapBeforeMergeToAfter[vBeforeMergeCounter] = vAfterMergeCounter;

            if(!alreadyAdded) {
                newVert -> ID = vAfterMergeCounter;
                mesh.addVertex(newVert);
                mapBeforeMergeToAfter[vBeforeMergeCounter] = vAfterMergeCounter;
                vAfterMergeCounter += 1;
            }
            vBeforeMergeCounter += 1;
        } else if(regex_match(nextLine, tRegex)) {
            string temp;
            temp = nextLine.substr(nextLine.find("\("), nextLine.find("\)") - nextLine.find("\("));
            //cout<<temp<<endl;
            temp = temp.substr(temp.find(" ") + 1);
            int a = stoi(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            int b = stoi(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            int c = stoi(temp);
            if(shellNum > 0) {
                vector<int>::iterator vertNumIt;
                IDplusBecauseOfShells = 0;
                for(vertNumIt = numberOfVerticesInShells.begin();
                 vertNumIt < numberOfVerticesInShells.end() - 1; vertNumIt ++) {
                    IDplusBecauseOfShells += *vertNumIt;
                }
            }
            a += IDplusBecauseOfShells;
            b += IDplusBecauseOfShells;
            c += IDplusBecauseOfShells;
            //cout<<"a: "<< a <<" b: "<<b<<" c: "<<c<<endl;
            a = mapBeforeMergeToAfter[a];
            b = mapBeforeMergeToAfter[b];
            c = mapBeforeMergeToAfter[c];
            Vertex * va = mesh.vertList[a];
            Vertex * vb = mesh.vertList[b];
            Vertex * vc = mesh.vertList[c];
            //cout<<va -> ID<<" "<<vb -> ID<<" "<<vc -> ID<<endl;
            mesh.addTriFace(va, vb, vc);
        } else if(regex_match(nextLine, lRegex)){
            vector<int> oneBoundary;
            string temp;
            int nextVert;
            if(shellNum > 1) {
                vector<int>::iterator vertNumIt;
                IDplusBecauseOfShells = 0;
                for(vertNumIt = numberOfVerticesInShells.begin();
                 vertNumIt < numberOfVerticesInShells.end(); vertNumIt ++) {
                    IDplusBecauseOfShells += *vertNumIt;
                }
            }
            temp = nextLine.substr(nextLine.find("\("),
             nextLine.find("\)") - nextLine.find("\("));
            //cout<<temp<<endl;
            temp = temp.substr(temp.find(" ") + 1);
            while(temp.find(" ") != string::npos){
                nextVert = stoi(temp.substr(0, temp.find(" ")));
                oneBoundary.push_back(nextVert + IDplusBecauseOfShells);
                temp = temp.substr(temp.find(" ") + 1);
            }
            nextVert = stoi(temp);
            oneBoundary.push_back(nextVert + IDplusBecauseOfShells);
            boundaries.push_back(oneBoundary);
            //cout<<oneBoundary.size()<<endl;          
        } else if(regex_match(nextLine, shRegex)) {
            shellNum += 1;
        } else if(regex_match(nextLine, verticesRegex)){
            string temp;
            temp = nextLine.substr(nextLine.find("\("),
             nextLine.find("\)") - nextLine.find("\("));
            temp = temp.substr(temp.find(" ") + 1);
            int numberOfVerticesInThisShell = stoi(temp);
            numberOfVerticesInShells.push_back(numberOfVerticesInThisShell);
        }
    }
    mesh.buildBoundary();
}

void makeWithSIFWithMerge(Mesh &mesh, string inputSIF){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    ifstream file(inputSIF);
    if (!file.good()) {
        cout<<"THE PATH OF SIF FILE IS NOT VAILD.";
        exit(1); // exit if file not found
    }
    string nextLine;
    regex anyR(".*(.*).*");
    regex vRegex(".*\(v .*$\).*");
    regex tRegex(".*\(t .*\).*");
    regex lRegex(".*\(loop .*\).*");
    regex shRegex(".*\\(shell.*\).*");
    regex verticesRegex(".*\\(vertices .*\).*");
    int vBeforeMergeCounter = 0;
    int vAfterMergeCounter = 0;
    int IDplusBecauseOfShells = 0;
    vector<vector<int> > boundaries;
    unordered_map<int, int> mapBeforeMergeToAfter;
    int shellNum = 0;
    vector<int> numberOfVerticesInShells;
    while(getline(file, nextLine)){
        nextLine.pop_back();
        if(regex_match(nextLine, vRegex)){
            string temp;
            temp = nextLine.substr(nextLine.find("\("), nextLine.find("\)") - nextLine.find("\("));
            temp = temp.substr(temp.find(" ") + 1);
            float x = stof(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            float y = stof(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            float z = stof(temp);
            Vertex * newVert = new Vertex;
            newVert -> position = vec3(x, y, z) * 1.0f; // Can be modifed here to zoom in.
            newVert -> ID = vAfterMergeCounter;
            vector<Vertex*>::iterator vIt;
            //cout<<newVert -> ID<<"Vertex added"<<endl;
            //cout<<"I am mapping "<<vBeforeMergeCounter<<" to "<<vAfterMergeCounter<<endl;
            //mapBeforeMergeToAfter[vBeforeMergeCounter] = vAfterMergeCounter;
            vBeforeMergeCounter += 1;
        } else if(regex_match(nextLine, tRegex)) {
            string temp;
            temp = nextLine.substr(nextLine.find("\("), nextLine.find("\)") - nextLine.find("\("));
            //cout<<temp<<endl;
            temp = temp.substr(temp.find(" ") + 1);
            int a = stoi(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            int b = stoi(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            int c = stoi(temp);
            if(shellNum > 0) {
                vector<int>::iterator vertNumIt;
                IDplusBecauseOfShells = 0;
                for(vertNumIt = numberOfVerticesInShells.begin();
                 vertNumIt < numberOfVerticesInShells.end() - 1; vertNumIt ++) {
                    IDplusBecauseOfShells += *vertNumIt;
                }
            }
            a += IDplusBecauseOfShells;
            b += IDplusBecauseOfShells;
            c += IDplusBecauseOfShells;
            //cout<<"a: "<< a <<" b: "<<b<<" c: "<<c<<endl;
            a = mapBeforeMergeToAfter[a];
            b = mapBeforeMergeToAfter[b];
            c = mapBeforeMergeToAfter[c];
            Vertex * va = mesh.vertList[a];
            Vertex * vb = mesh.vertList[b];
            Vertex * vc = mesh.vertList[c];
            //cout<<va -> ID<<" "<<vb -> ID<<" "<<vc -> ID<<endl;
            mesh.addTriFace(va, vb, vc);
        } else if(regex_match(nextLine, lRegex)){
            vector<int> oneBoundary;
            string temp;
            int nextVert;
            if(shellNum > 1) {
                vector<int>::iterator vertNumIt;
                IDplusBecauseOfShells = 0;
                for(vertNumIt = numberOfVerticesInShells.begin();
                 vertNumIt < numberOfVerticesInShells.end(); vertNumIt ++) {
                    IDplusBecauseOfShells += *vertNumIt;
                }
            }
            temp = nextLine.substr(nextLine.find("\("),
             nextLine.find("\)") - nextLine.find("\("));
            //cout<<temp<<endl;
            temp = temp.substr(temp.find(" ") + 1);
            while(temp.find(" ") != string::npos){
                nextVert = stoi(temp.substr(0, temp.find(" ")));
                oneBoundary.push_back(nextVert + IDplusBecauseOfShells);
                temp = temp.substr(temp.find(" ") + 1);
            }
            nextVert = stoi(temp);
            oneBoundary.push_back(nextVert + IDplusBecauseOfShells);
            boundaries.push_back(oneBoundary);
            //cout<<oneBoundary.size()<<endl;          
        } else if(regex_match(nextLine, shRegex)) {
            shellNum += 1;
        } else if(regex_match(nextLine, verticesRegex)){
            string temp;
            temp = nextLine.substr(nextLine.find("\("),
             nextLine.find("\)") - nextLine.find("\("));
            temp = temp.substr(temp.find(" ") + 1);
            int numberOfVerticesInThisShell = stoi(temp);
            numberOfVerticesInShells.push_back(numberOfVerticesInThisShell);
        }
    }
    mesh.buildBoundary();
}

void makeWithQuadSIF(Mesh &mesh, string inputSIF){
    mesh.vertList.clear();
    mesh.edgeTable.clear();
    mesh.faceList.clear();
    ifstream file(inputSIF);
    if (!file.good()) {
        cout<<"THE PATH OF SIF FILE IS NOT VAILD.";
        exit(1); // exit if file not found
    }
    string nextLine;
    regex anyR(".*(.*).*");
    regex vRegex(".*\(v .*$\).*");
    regex tRegex(".*\(t .*\).*");
    regex lRegex(".*\(loop .*\).*");
    regex shRegex(".*\\(shell.*\).*");
    regex verticesRegex(".*\\(vertices .*\).*");
    int vBeforeMergeCounter = 0;
    int vAfterMergeCounter = 0;
    int IDplusBecauseOfShells = 0;
    vector<vector<int> > boundaries;
    unordered_map<int, int> mapBeforeMergeToAfter;
    int shellNum = 0;
    vector<int> numberOfVerticesInShells;
    int tCounter = 0;
    Vertex * va;
    Vertex * vb;
    Vertex * vc;
    Vertex * vd;
    while(getline(file, nextLine)){
        nextLine.pop_back();
        if(regex_match(nextLine, vRegex)){
            string temp;
            temp = nextLine.substr(nextLine.find("\("), nextLine.find("\)") - nextLine.find("\("));
            temp = temp.substr(temp.find(" ") + 1);
            float x = stof(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            float y = stof(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            float z = stof(temp);
            Vertex * newVert = new Vertex;
            newVert -> position = vec3(x, y, z) * 1.0f; // Can be modifed here to zoom in.
            newVert -> ID = vAfterMergeCounter;
            vector<Vertex*>::iterator vIt;
            bool alreadyAdded = false;
            for (vIt = mesh.vertList.begin(); vIt < mesh.vertList.end(); vIt ++) {
                if(distance(newVert -> position, (*vIt) -> position) < 0.001 ){
                    //cout << "The distance between vertex "<<newVert -> ID<<" and vertex "<<vIt -> second -> ID<<" is: "<<endl;
                    //cout << newVert -> position - (*vIt) -> position<<endl;
                    alreadyAdded = true;
                    mapBeforeMergeToAfter[vBeforeMergeCounter] = (*vIt) -> ID;
                    break;
                }
            }
            if(!alreadyAdded) {
                newVert -> ID = vAfterMergeCounter;
                mesh.addVertex(newVert);
                mapBeforeMergeToAfter[vBeforeMergeCounter] = vAfterMergeCounter;
                vAfterMergeCounter += 1;
            }
            vBeforeMergeCounter += 1;
        } else if(regex_match(nextLine, tRegex)) {
            string temp;
            temp = nextLine.substr(nextLine.find("\("), nextLine.find("\)") - nextLine.find("\("));
            //cout<<temp<<endl;
            temp = temp.substr(temp.find(" ") + 1);
            int a = stoi(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            int b = stoi(temp.substr(0, temp.find(" ")));
            temp = temp.substr(temp.find(" ") + 1);
            int c = stoi(temp);
            if(shellNum > 0) {
                vector<int>::iterator vertNumIt;
                IDplusBecauseOfShells = 0;
                for(vertNumIt = numberOfVerticesInShells.begin();
                 vertNumIt < numberOfVerticesInShells.end() - 1; vertNumIt ++) {
                    IDplusBecauseOfShells += *vertNumIt;
                }
            }
            a += IDplusBecauseOfShells;
            b += IDplusBecauseOfShells;
            c += IDplusBecauseOfShells;
            //cout<<"a: "<< a <<" b: "<<b<<" c: "<<c<<endl;
            a = mapBeforeMergeToAfter[a];
            b = mapBeforeMergeToAfter[b];
            c = mapBeforeMergeToAfter[c];
            //cout<<"a: "<< a <<" b: "<<b<<" c: "<<c<<endl;
            if(tCounter % 2 == 0) {
                va = mesh.vertList[a];
                vb = mesh.vertList[b];
                vc = mesh.vertList[c];
            } else {
                vd = mesh.vertList[c];
                //cout<<va -> ID<<" "<<vb -> ID<<" "<<vc -> ID<<" "<<vd -> ID<<endl;
                mesh.addQuadFace(va, vb, vc, vd);
            }
            tCounter += 1;
            //cout<<va -> ID<<" "<<vb -> ID<<" "<<vc -> ID<<endl;
        } else if(regex_match(nextLine, lRegex)){
            vector<int> oneBoundary;
            string temp;
            int nextVert;
            if(shellNum > 1) {
                vector<int>::iterator vertNumIt;
                IDplusBecauseOfShells = 0;
                for(vertNumIt = numberOfVerticesInShells.begin();
                 vertNumIt < numberOfVerticesInShells.end(); vertNumIt ++) {
                    IDplusBecauseOfShells += *vertNumIt;
                }
            }
            temp = nextLine.substr(nextLine.find("\("),
             nextLine.find("\)") - nextLine.find("\("));
            //cout<<temp<<endl;
            temp = temp.substr(temp.find(" ") + 1);
            while(temp.find(" ") != string::npos){
                nextVert = stoi(temp.substr(0, temp.find(" ")));
                oneBoundary.push_back(nextVert + IDplusBecauseOfShells);
                temp = temp.substr(temp.find(" ") + 1);
            }
            nextVert = stoi(temp);
            oneBoundary.push_back(nextVert + IDplusBecauseOfShells);
            boundaries.push_back(oneBoundary);
            //cout<<oneBoundary.size()<<endl;          
        } else if(regex_match(nextLine, shRegex)) {
            shellNum += 1;
        } else if(regex_match(nextLine, verticesRegex)){
            string temp;
            temp = nextLine.substr(nextLine.find("\("),
             nextLine.find("\)") - nextLine.find("\("));
            temp = temp.substr(temp.find(" ") + 1);
            int numberOfVerticesInThisShell = stoi(temp);
            numberOfVerticesInShells.push_back(numberOfVerticesInThisShell);
        }
    }
    mesh.buildBoundary();
}

#endif // __MAKEMESH_H__