#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "model.h"
#include "mymathlib.h"

enum Type{
    COMMON_OBJECT,
    BLEND_OBJECT,
    EMPTY,
};

class GameObject{
public:
    GameObject():quaternion(Quat()), type(EMPTY){
        local_position = glm::vec3(0.0f, 0.0f, 0.0f);
        scale_rate = glm::vec3(1.0f, 1.0f, 1.0f);
        model = NULL;
        shader = NULL;
        parent = NULL;
    }
    GameObject(string const &path, Shader* ss, Type tt = COMMON_OBJECT) :model(new Model(path)), quaternion(Quat()), type(tt){
        local_position = glm::vec3(0.0f, 0.0f, 0.0f);
        scale_rate = glm::vec3(1.0f, 1.0f, 1.0f);
        shader = ss;
        parent = NULL;
    }
    ~GameObject(){
        delete(model);
    }
    
    void draw(float time);
    void add(GameObject* child);
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setScale(glm::vec3 pos);
    glm::vec3 getScale();
    void setQuaternion(Quat pos);
    Quat getQuaternion();

private:
    GameObject* parent;
    glm::vec3 local_position;
    glm::vec3 scale_rate;
    Quat quaternion;
    Type type;
    Model* model;
    Shader* shader;
	glm::mat4 getModelMat(GameObject* root);
	mymathlib::Mat4 getModelMat1(GameObject* root);
};
#endif