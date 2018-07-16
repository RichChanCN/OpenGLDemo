#include "game.h"
void Game::initWindowEvent(){
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::start(){
	Shader* shader = new Shader("shaders\\load_model_vs.glsl", "shaders\\load_model_fs.glsl");
    Shader* blend_shader = new Shader("shaders\\load_model_vs.glsl", "shaders\\blend_fs.glsl");

	Model* nanosuit = new Model("assets\\models\\nanosuit\\nanosuit2.fbx", shader);
	nanosuit->loadAnimation("assets\\models\\nanosuit\\animation2_without_skin.fbx", "anim2");
	nanosuit->loadAnimation("assets\\models\\nanosuit\\Walking_without_skin.fbx", "walk");
    nanosuit->setEffectType(BORDER);
	scene.add(nanosuit);
	setCtrlModel(nanosuit);
    
    Model* plane = Model::plane(shader);
    plane->setScale(glm::vec3(2.0));
    scene.add(plane);

    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 1.0f, -5.0f));
    vegetation.push_back(glm::vec3(4.5f, 1.0f, -0.0f));
    vegetation.push_back(glm::vec3(0.0f, 1.0f, -7.0f));
    vegetation.push_back(glm::vec3(-3.0f, 1.0f, -2.0f));
    vegetation.push_back(glm::vec3(0.5f, 1.0f, -6.0f));
    for (int i = 0; i < 5; i++)
    {
        Model* vertical_planes = Model::vertical_plane(blend_shader);
        vertical_planes->setScale(glm::vec3(2.0));
        vertical_planes->setPosition(vegetation[i]);
        scene.add(vertical_planes);
    }

    scene.add(Light(glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	scene.add(shader);
}

void Game::update(){

    for (int i = 0; i < 5; i++)
    {
        glm::vec3 pos = scene.transparent_object_list[i]->getPosition();
        scene.transparent_object_list[i]->setPosition(glm::vec3(sin(lastFrame + i) * 5, pos.y, pos.z));
    }

	processInput(window);
	ModelCtrl(window);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		instance->scene.camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		instance->scene.camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		instance->scene.camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		instance->scene.camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		instance->scene.camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		instance->scene.camera.ProcessKeyboard(DOWN, deltaTime);
}
void Game::ModelCtrl(GLFWwindow *window)
{
	if (instance->ctrl_object == NULL)
		return;
	glm::vec3 pos = instance->ctrl_object->getPosition();
	glm::vec3 scale = instance->ctrl_object->getScale();
	Quat quat = instance->ctrl_object->getQuaternion();
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		instance->ctrl_object->setPosition(glm::vec3(pos.x + deltaTime * 4, pos.y, pos.z));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		instance->ctrl_object->setPosition(glm::vec3(pos.x - deltaTime * 4, pos.y, pos.z));
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		instance->ctrl_object->changeAnimation("walk");
		instance->ctrl_object->setPosition(glm::vec3(pos.x, pos.y, pos.z + deltaTime * 4));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		instance->ctrl_object->setPosition(glm::vec3(pos.x, pos.y, pos.z - deltaTime * 4));
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		instance->ctrl_object->setScale(scale + deltaTime / 2);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		instance->ctrl_object->setScale(scale - deltaTime / 2);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		instance->ctrl_object->setQuaternion(Quat(quat.x, quat.y, quat.z, quat.w + deltaTime * 2));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		instance->ctrl_object->setQuaternion(Quat(quat.x, quat.y, quat.z, quat.w - deltaTime * 2));
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		instance->ctrl_object->changeAnimation("anim2");
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	instance->scene.camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	instance->scene.camera.ProcessMouseScroll(yoffset);
}