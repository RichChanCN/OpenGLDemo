#include "gameobject.h"

void GameObject::add(GameObject* child){
    child->parent = this;
}

void GameObject::draw(){
    if (model == NULL || type == EMPTY || shader == NULL)
        return;
    glm::mat4 model_mat = getModelMat(this);
    shader->setMat4("model", model_mat);

    model->Draw(shader);
}

glm::mat4 GameObject::getModelMat(GameObject* root){
    GameObject* p = root;
    if (p->parent == NULL){
        return glm::rotate(glm::scale(glm::translate(glm::mat4(), p->local_position), p->scale_rate), p->quaternion.w, p->quaternion.axis());
    }

    return glm::rotate(glm::scale(glm::translate(getModelMat(p->parent), p->local_position), p->scale_rate), p->quaternion.w, p->quaternion.axis());
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