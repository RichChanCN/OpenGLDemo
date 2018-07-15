#include "gameobject.h"
void GameObject::add(GameObject* child){
    child->parent = this;
}

void GameObject::draw(float time){
    if (model == NULL || type == EMPTY || shader == NULL)
        return;
    glm::mat4 model_mat = getModelMat1(this).tomat4();
    shader->setMat4("model", model_mat);

    model->Draw(shader, time);
}
Model* GameObject::getModel(){
    return model;
}
/*
glm::mat4 GameObject::getModelMat(GameObject* root){
    GameObject* p = root;
    if (p->parent == NULL){
        return glm::rotate(glm::scale(glm::translate(glm::mat4(), p->local_position), p->scale_rate), p->quaternion.w, p->quaternion.axis());
    }

    return glm::rotate(glm::scale(glm::translate(getModelMat(p->parent), p->local_position), p->scale_rate), p->quaternion.w, p->quaternion.axis());
}
*/
mymathlib::Mat4 GameObject::getModelMat1(GameObject* root){
	GameObject* p = root;
	if (p->parent == NULL){
		return mymathlib::rotate(mymathlib::scale(mymathlib::translate(mymathlib::Mat4(), p->local_position), p->scale_rate), p->quaternion);
	}

	return mymathlib::rotate(mymathlib::scale(mymathlib::translate(p->getModelMat1(p->parent), p->local_position), p->scale_rate), p->quaternion);
}

void GameObject::setPosition(glm::vec3 pos){
    local_position = pos;
}

glm::vec3 GameObject::getPosition(){
    return local_position;
}

void GameObject::setScale(glm::vec3 rate){
    scale_rate = rate;
}

glm::vec3 GameObject::getScale(){
    return scale_rate;
}

void GameObject::setQuaternion(Quat quat){
    quaternion = quat;
}

Quat GameObject::getQuaternion(){
    return quaternion;
}