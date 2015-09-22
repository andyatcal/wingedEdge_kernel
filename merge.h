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
class Merge{
public:
    // Merge any possible boundary edges that are close for one mesh.
    // @param mesh, the mesh of self-merege
    Mesh selfMerge(Mesh & mesh);
    // Merge any possible boundary edges that are close for two meshes.
    // @param mesh1, mesh2. The two meshes to be merged.
    // mesh2 will disappear after the merge.
    // All facets and vertices of mesh2 will be added to mesh1.
    Mesh merge(Mesh & mesh1, Mesh & mesh2);
    // Merge two edges into a regular edge. 
    // @param edge1, edge2. The two edges to be merged.
    // edge1 remains in the merged mesh, edge2 is deleted.
    // @param mode. MODE determines the method of merge.
    // When MODE = 0, the positions of vertices in edge1 do not change.
    // When MODE = 1, the positions of vertices in edge1 change to
    // mid points between edge1 and edge2.
    // Can add more later....
    void mergeRegularEdge(Edge * edge1, Edge * edge2, int mode);
    // Merge two edges into a mobius edge. 
    // @param edge1, edge2. The two edges to be merged.
    // edge1 remains in the merged mesh, edge2 is deleted.
    // @param mode. MODE determines the method of merge.
    // When MODE = 0, the positions of vertices in edge1 do not change.
    // When MODE = 1, the positions of vertices in edge1 change to
    // mid points between edge1 and edge2.
    // Can add more later....
    void mergeMobiusEdge(Edge * edge1, Edge * edge2, int mode);
private:
    // Find matching edges in mesh1 and mesh2
    // @param mesh1, mesh2. The two meshes to be merged.
    // ???????
    void findMatchingEdges(Mesh mesh1, Mesh mesh2);
};

Mesh Merge::merge(Mesh & mesh1, Mesh & mesh2) {
    return mesh1;
}

Mesh Merge::selfMerge(Mesh & mesh) {
    return mesh;
}

void Merge::mergeRegularEdge(Edge * edge1, Edge * edge2, int mode = 0) {
    // Two steps. 1. Change the pointers of edge1. 
    // 2. All previous pointers pointing to edge2 should now pointing to edge1 or ?
    edge1 -> fb = edge2 -> fa;
    edge1 -> nextVaFb = edge2 -> nextVbFa;
    edge1 -> nextVbFb = edge2 -> nextVaFa;
    edge2 -> nextVbFa -> nextVaFa = edge1;
    edge2 -> nextVaFa -> nextVbFa = edge1;
    edge2 -> fa -> oneEdge = edge1;
}

void Merge::mergeMobiusEdge(Edge * edge1, Edge * edge2, int mode = 0) {
    edge1 -> fb = edge2 -> fa;
    edge1 -> nextVaFb = edge2 -> nextVaFa;
    edge1 -> nextVbFb = edge2 -> nextVbFa;
}

#endif // __MESH_H__