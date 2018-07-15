#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
class Light{
public:
    Light(glm::vec3 pos, glm::vec3 color){
        light_pos = pos;
        light_color = color;
        diffuse_color = light_color   * glm::vec3(0.5f);
        ambient_color = diffuse_color * glm::vec3(0.2f);
    }
    glm::vec3 light_pos;
    //light configure
    glm::vec3 light_color;
    glm::vec3 diffuse_color;
    glm::vec3 ambient_color;

    void update(Shader shader);
};

#endif