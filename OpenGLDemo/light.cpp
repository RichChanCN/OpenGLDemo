#include"light.h"

void Light::update(Shader* shader){
    shader->setVec3("light.ambient", ambient_color);
	shader->setVec3("light.diffuse", diffuse_color);
	shader->setVec3("light.specular", light_color);
	shader->setVec3("light.position", light_pos);
}