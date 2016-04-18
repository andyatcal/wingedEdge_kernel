// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

using namespace std;
using namespace glm;

// Forward declarations
class Group;

//////////////////////////////////////////////////////////////////////
// Scene Class -- A scene is a collection of groups and lighting conditions
class Scene{
public:
    // Constructor.
    Scene();
    // Add one group of drawing elements to this scene.
    void addGroup();
    // The instances in this scene
};

Scene::Scene() {}

#endif // __SCENE_H__