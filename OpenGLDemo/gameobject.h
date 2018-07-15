/*
场景中物体的基类，暂时只被模型继承，后面考虑把灯光，相机等都继承此类
*/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "shader.h"
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
        shader = NULL;
        parent = NULL;
    }
    GameObject(Shader* ss, Type tt = EMPTY) : quaternion(Quat()), type(tt){
        local_position = glm::vec3(0.0f, 0.0f, 0.0f);
        scale_rate = glm::vec3(1.0f, 1.0f, 1.0f);
        shader = ss;
        parent = NULL;
    }
	virtual ~GameObject(){
		parent = NULL;
		shader = NULL;
	}
    
    virtual void draw(float time);
    void add(GameObject* child);
    void setPosition(glm::vec3 pos);
    glm::vec3 getPosition();
    void setScale(glm::vec3 pos);
    glm::vec3 getScale();
    void setQuaternion(Quat pos);
	Quat getQuaternion();
	Type getType();
	glm::vec3 getWorldPosition();

protected:
    GameObject* parent;
    glm::vec3 local_position;
    glm::vec3 scale_rate;
    Quat quaternion;
    Type type;
    Shader* shader;
	glm::mat4 getModelMat(GameObject* root);
	mymathlib::Mat4 getModelMat1(GameObject* root);
};
#endif