#include "scene.h"

void Scene::render(unsigned int src_width, unsigned int src_height){
    for (int i = 0; i < light_list.size(); i++)
    {
        for (int j = 0; j < shader_list.size(); j++)
        {
            light_list[i].update(shader_list[j]);
        }
    }
    // set uniforms
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)src_width / (float)src_height, 0.1f, 100.0f);

    for (int i = 0; i < shader_list.size(); i++)
    {
        shader_list[i].use();
        shader_list[i].setMat4("view", view);
        shader_list[i].setMat4("projection", projection);
        shader_list[i].setVec3("viewPos", camera.Position);
    }
    for (int i = 0; i < common_object_list.size(); i++)
    {
        common_object_list[i]->draw();
    }
    for (int i = 0; i < transparent_object_list.size(); i++)
    {
        transparent_object_list[i]->draw();
    }
}