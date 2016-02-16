// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __GROUP_H__
#define __GROUP_H__

#include <vector>
#include <unordered_map>

using namespace std;
using namespace glm;

// Forward declarations
class Mesh;

//////////////////////////////////////////////////////////////////////
// Mesh Class -- A MESH is a collection of polygon facets
class Group{
public:
    // Constructor.
    Group();
    // Add one mesh to the current group
    // @param vertices is a list of consequtive vertices of the face.
    void addMesh(Mesh* mesh, mat4 transformation);
    // @param vertices is a list of consequtive vertices of the face.
    void removeMesh(Mesh* mesh);
    // Now think about what instances of this group would have?
    // It has a collection of mesh pointers and the transformations
    // for each one of them.
    unordered_map<Mesh*, mat4> meshes;
    // Also, it contains pointers to the subgroups so we can build
    // hirachinal scene.
    vector<Group*> subgroups;
    // The hashMap of this group
};

Group::Group() {}

void Group::addMesh(Mesh* mesh, mat4 transformation) {
    meshes[mesh] = transformation;
}

void Group::removeMesh(Mesh* mesh) {
    meshes.erase(mesh);
}
#endif // __GROUP_H__