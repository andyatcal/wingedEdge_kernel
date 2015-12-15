
// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __MAKEPOLYLINE_H__
#define __MAKEPOLYLINE_H__

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace glm;
using namespace std;

//////////////////////////////////////////////////////////////////////
// MakePolyline Class -- Create Initial Meshes.
Mesh initPolyline()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    //boarder1 -> isLoop = true;
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
    glMesh = zipper.zip_skewness(boarder1, boarder2);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline1()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    glMesh = zipper.zip_skewness(boarder1, boarder2, 2.0);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline2()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    glMesh = zipper.zip_skewness(boarder1, boarder2, 1.0);
    glMesh.computeNormals();
    return glMesh;
}


Mesh initPolyline4()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    glMesh = zipper.zip_skewness(boarder1, boarder2);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline8()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    glMesh = zipper.zip_skewness(boarder1, boarder2, 1.0);
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(2);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline9()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    boarder2 -> vertices.push_back(v25);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v20);
    Zipper zipper;
    glMesh = zipper.zip_skewness(boarder1, boarder2, 1.0);
    Subdivision myCC(glMesh);
    glMesh = myCC.ccSubdivision(2);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline3()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    //boarder1 -> isLoop = true;
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
    boarder2 -> vertices.push_back(v25);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v20);
    Zipper zipper;
    glMesh = zipper.zip_skewness(boarder1, boarder2);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline5()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    v10 -> position = vec3(-1, 1, 1);
    v11 -> position = vec3(-1, 1, 0);
    v12 -> position = vec3(-1, 1, -1);
    v13 -> position = vec3(1, 1, -1);
    v14 -> position = vec3(1, 1, 0);
    v15 -> position = vec3(1, 1, 1);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);
    boarder1 -> vertices.push_back(v15);
    boarder1 -> isLoop = true;
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
    v20 -> position = vec3(-1, -1, 1);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-1, -1, -1);
    v23 -> position = vec3(1, -1, -1);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(1, -1, 1);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v25);
    boarder2 -> isLoop = true;
    Zipper zipper;
    glMesh = zipper.zip_skewness(boarder1, boarder2, 1.0);
    glMesh.computeNormals();
    return glMesh;
}

Mesh initPolyline6()
{
    Mesh glMesh;
    Polyline * boarder1 = new Polyline;
    Polyline * boarder2 = new Polyline;
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
    v10 -> position = vec3(-1, 1, 1);
    v11 -> position = vec3(-1, 1, 0);
    v12 -> position = vec3(-1, 1, -1);
    v13 -> position = vec3(1, 1, -1);
    v14 -> position = vec3(1, 1, 0);
    v15 -> position = vec3(1, 1, 1);
    boarder1 -> vertices.push_back(v10);
    boarder1 -> vertices.push_back(v11);
    boarder1 -> vertices.push_back(v12);
    boarder1 -> vertices.push_back(v13);
    boarder1 -> vertices.push_back(v14);
    boarder1 -> vertices.push_back(v15);
    boarder1 -> isLoop = true;
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
    v20 -> position = vec3(-1, -1, 1);
    v21 -> position = vec3(-1, -1, 0);
    v22 -> position = vec3(-1, -1, -1);
    v23 -> position = vec3(1, -1, -1);
    v24 -> position = vec3(1, -1, 0);
    v25 -> position = vec3(1, -1, 1);
    boarder2 -> vertices.push_back(v20);
    boarder2 -> vertices.push_back(v25);
    boarder2 -> vertices.push_back(v24);
    boarder2 -> vertices.push_back(v23);
    boarder2 -> vertices.push_back(v22);
    boarder2 -> vertices.push_back(v21);
    boarder2 -> isLoop = true;
    Zipper zipper;
    glMesh = zipper.zip_skewness(boarder1, boarder2, 1.0);
    glMesh.computeNormals();
    return glMesh;
}

#endif // __MAKEPOLYLINE_H__