// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __EDGE_H__
#define __EDGE_H__

using namespace glm;
using namespace std;

// Forward declarations
class Vertex;
class Face;

//////////////////////////////////////////////////////////////////////
// Edge Class -- Edge connects Vertex A and Vertex B.
//
class Edge{
public:
	// Pointer to Vertex A of this edge.
    Vertex * va;
    // Pointer to Vertex B of this edge.
    Vertex * vb;
    // Pointer to Face A of this edge.
    Face * fa;
    // Pointer to Face B of this edge.
    Face * fb;
    // Next edge on the side of Va and Fa.
    Edge * nextVaFa;
    // Next edge on the side of Va and Fb.
    Edge * nextVaFb;
    // Next edge on the side of Vb and Fa.
    Edge * nextVbFa;
    // Next edge on the side of Vb and Fb.
    Edge * nextVbFb;
    // A pointer to the edge point after makeEdgePoints in subdivision.
    Vertex * edgePoint;
    // A pointer to the first half edge in subdivision.
    Edge * firstHalf;
    // A pointer to the second half edge in subdivision.
    Edge * secondHalf;
    // A flag of mobius. If two faces are with same orientation, false. Otherwise, true.
    bool mobius;
    // True if this edge is sharp in subdivision.
    bool isSharp;
    // Constructor.
    Edge();
    // Constructor with start and end vertex.
    // @param va, vb: pointers to the va and vb of this edge.
    Edge(Vertex * va, Vertex * vb);
    // Find the corresponding next edge given a vertex and face of this edge.
    // Return an error if the vertex or face is not adjacent to this edge.
    Edge * nextEdge(Vertex * v, Face * f);
    // Set the corresponding next edge with a vertex and face of this edge.
    // Return an error if the vertex or face is not adjacent to this edge.
    void setNextEdge(Vertex * v, Face * f, Edge * nextEdge);
    // Find the other point of this edge
    // @param v, the known vertex
    Vertex * theOtherVertex(Vertex * v);
    // Find the other face of this edge
    // @param f, the known face
    Face * theOtherFace(Face * f);
    // The next edge in a face traversal of f.
    Edge * nextEdgeOfFace(Face * f);
};

Edge::Edge() {
    va = vb = NULL;
    nextVaFa = nextVaFb = nextVbFa = nextVbFb = NULL;
    fa = fb = NULL;
    edgePoint = NULL;
    isSharp = false;
    mobius = false;
    firstHalf = secondHalf = NULL;
}

Edge::Edge(Vertex * v1, Vertex * v2) {
    va = v1;
    vb = v2;
    nextVaFa = nextVaFb = nextVbFa = nextVbFb = NULL;
    fa = fb = NULL;
    edgePoint = NULL;
    isSharp = false;
    mobius = false;
    firstHalf = secondHalf = NULL;
}

Edge * Edge::nextEdge(Vertex * v, Face * f) {
    if(v == va) {
        if(f == fa) {
            return nextVaFa;
        } else if (f == fb) {
            return nextVaFb;
        }
    } else if(v == vb) {
        if(f == fa) {
            return nextVbFa;
        } else if (f == fb) {
            return nextVbFb;
        }
    }
    cout<<"Error: Invalid search of edge at vertex "<<v -> ID<<"."<<endl;
    exit(0);
}

void Edge::setNextEdge(Vertex * v, Face * f, Edge * nextEdge) {
    if(v == va) {
        if(f == fa) {
            nextVaFa = nextEdge;
            return;
        } else if (f == fb) {
            nextVaFb = nextEdge;
            return;
        }
    } else if(v == vb) {
        if(f == fa) {
            nextVbFa = nextEdge;
            return;
        } else if (f == fb) {
            nextVbFb = nextEdge;
            return;
        }
    }
    cout<<"Error: Invalid set next edge at vertex "<<v -> ID<<"."<<endl;
    exit(0);
}

Vertex * Edge::theOtherVertex(Vertex * v) {
    if(v == va) {
        return vb;
    } else if(v == vb) {
        return va;
    } else {
        cout<<"ERROR: v is not in edge!"<<endl;
        exit(0);
    }
}

Face * Edge::theOtherFace(Face * f) {
    if(f == fa) {
        return fb;
    } else if(f == fb) {
        return fa;
    } else {
        cout<<"ERROR: f is not adjacent to this edge"<<endl;
        exit(0);
    }
}

Edge * Edge::nextEdgeOfFace(Face * f) {
    if(f == fa) {
        return nextVbFa;
    } else if(f == fb) {
        if(mobius) {
            return nextVbFb;
        } else {
            return nextVaFb;
        }
    } else {
        cout<<"ERROR: f is not adjacent to this edge"<<endl;
        exit(0);
    }
}

#endif // __EDGE_H__