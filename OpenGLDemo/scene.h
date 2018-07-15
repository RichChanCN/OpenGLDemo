#ifndef SCENE_H
#define SCENE_H
#include<memory>
#include<vector>

#include"gameobject.h"
#include"shader.h"
#include"camera.h"
#include"light.h"

class Scene{
public:
    Scene(){
    }
    Camera camera;
    vector<Light> light_list;
    vector<Shader> shader_list;
    vector<GameObject *> common_object_list;
    vector<GameObject *> transparent_object_list;
    void render(unsigned int src_width, unsigned int src_height, float time);
};

#endif