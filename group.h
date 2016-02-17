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
// Group Class -- A Group is a collection of meshes and its subgroups
class Group{
public:
    // Constructor.
    Group();
    // Add one mesh to the current group
    // @param mesh, the mesh to be added.
    // @param transformation, the transformation of this mesh.
    void addMesh(Mesh* mesh, mat4 transformation);
    // Remove one mesh to the current group
    // @param vertices is a list of consequtive vertices of the face.
    void removeMesh(Mesh* mesh);
    // add subgroup for this Group
    void addSubGroup(Group* group, mat4 transformation);
    // A group is a collection of mesh pointers and the transformations
    // for each one of them.
    unordered_map<Mesh*, mat4> meshes;
    // Also, it contains pointers to the subgroups so we can build
    // hirachinal scene.
    unordered_map<Group*, mat4> subgroups;
    // It can also have a special color of this group;
};

Group::Group() {}

void Group::addMesh(Mesh* mesh, mat4 transformation) {
    meshes[mesh] = transformation;
}

void Group::removeMesh(Mesh* mesh) {
    meshes.erase(mesh);
}

    void addGroup(Group* group);
#endif // __GROUP_H__