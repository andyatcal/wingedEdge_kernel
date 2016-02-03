// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////
// Transformation Class -- A class to create a new mesh by transformation.
    
// Create a new Mesh given the transformation matrix
// @param mesh, the mesh for transformation
// @param matrix, the matrix for transformation
void transform(Mesh & mesh, mat4 matrix) {
    vector<Vertex*>::iterator vIt;
    for(vIt = mesh.vertList.begin(); vIt < mesh.vertList.end(); vIt++) {
        (*vIt) -> position = vec3(matrix * vec4((*vIt) -> position, 1));
    }
}

mat4 krotate(vec3 rot_axis, float radian) {

    return rotate(radian, rot_axis);
}

mat4 kscale(vec3 scales) {
    return scale(scales);
}

mat4 ktranslate(vec3 translation) {
    return translate(translation);
}

#endif // __TRANSFORMATION_H__