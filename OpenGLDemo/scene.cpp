#include "scene.h"
Scene::Scene(){
	camera = Camera(glm::vec3(0.0, 8.0, 20.0));
}

Scene::~Scene(){
	for (size_t i = 0; i < shader_list.size(); i++)
	{
		delete(shader_list[i]);
		shader_list[i] = NULL;
	}
	shader_list.clear();

	for (size_t i = 0; i < common_object_list.size(); i++)
	{
		delete(common_object_list[i]);
		common_object_list[i] = NULL;
	}
	common_object_list.clear();

	for (size_t i = 0; i < transparent_object_list.size(); i++)
	{
		delete(transparent_object_list[i]);
		transparent_object_list[i] = NULL;
	}
	transparent_object_list.clear();

	for (size_t i = 0; i < empty_object_list.size(); i++)
	{
		delete(empty_object_list[i]);
		empty_object_list[i] = NULL;
	}
	empty_object_list.clear();

}

void Scene::render(unsigned int src_width, unsigned int src_height, float time){
    for (unsigned int i = 0; i < light_list.size(); i++)
    {
        for (unsigned int j = 0; j < shader_list.size(); j++)
        {
            light_list[i].update(shader_list[j]);
        }
    }
    // set uniforms
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)src_width / (float)src_height, 0.1f, 100.0f);

    for (unsigned int i = 0; i < shader_list.size(); i++)
    {
        shader_list[i]->use();
		shader_list[i]->setMat4("view", view);
		shader_list[i]->setMat4("projection", projection);
		shader_list[i]->setVec3("viewPos", camera.Position);
    }

	//先渲染不透明物体
    for (unsigned int i = 0; i < common_object_list.size(); i++)
    {
        common_object_list[i]->draw(time);
    }

	//透明物体排序后渲染
	std::map<float, int> sorted;
	for (unsigned int i = 0; i < transparent_object_list.size(); i++)
	{
		float distance = glm::length(camera.Position - transparent_object_list[i]->getWorldPosition());
		sorted[distance] = i;
	}
	for (std::map<float, int>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		transparent_object_list[it->second]->draw(time);
	}
}

void Scene::add(GameObject* go){
	switch (go->getType())
	{
	case COMMON_OBJECT:
		common_object_list.push_back(go);
		break;
	case BLEND_OBJECT:
		transparent_object_list.push_back(go);
		break;
	case EMPTY:
		empty_object_list.push_back(go);
		break;
	default:
		break;
	}
}

void Scene::add(Shader* shader){
	shader_list.push_back(shader);
}

void Scene::add(Light light){
	light_list.push_back(light);
}