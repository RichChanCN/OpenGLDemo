#include"game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int main(){
    Game* game = Game::Instance();
    Shader shader("shaders\\load_model_vs.glsl", "shaders\\load_model_fs.glsl");
    GameObject* nanosuit = new GameObject("assets\\models\\nanosuit\\nanosuit2.fbx",&shader);
    nanosuit->getModel()->loadAnimation("assets\\models\\nanosuit\\animation2_without_skin.fbx", "anim2");
    nanosuit->getModel()->loadAnimation("assets\\models\\nanosuit\\Walking_without_skin.fbx", "walk");
    game->setCtrlModel(nanosuit);
    game->scene->light_list.push_back(Light(glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    game->scene->common_object_list.push_back(nanosuit);
    game->scene->shader_list.push_back(shader);
    game->gameloop();
}