#include "game.h"
Game* Game::instance = NULL;
GameObject* Game::ctrl_object = NULL;
// timing
float Game::deltaTime = 0.0f;
float Game::lastFrame = 0.0f;

float Game::lastX = 0.0f;
float Game::lastY = 0.0f;
bool Game::firstMouse = true;

Game* Game::Instance(){
    if (instanceCheck())
        return instance;
    else{
        instance = new Game();
        return instance;
    }
}

bool Game::instanceCheck(){
    return (instance != NULL);
}
void Game::setCtrlModel(GameObject* go){
    instance->ctrl_object = go;
}
void Game::gameloop(){
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        ModelCtrl(window);
        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->render(src_width, src_height);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glfwTerminate();
    return;
}

void Game::initWindow(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(src_width, src_height, "OpenGLDemo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        instance->scene->camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        instance->scene->camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        instance->scene->camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        instance->scene->camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        instance->scene->camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        instance->scene->camera.ProcessKeyboard(DOWN, deltaTime);
}
void Game::ModelCtrl(GLFWwindow *window)
{
    if (instance->ctrl_object == NULL)
        return;
    glm::vec3 pos = instance->ctrl_object->getPosition();
    glm::vec3 scale = instance->ctrl_object->getScale();
    Quat quat = instance->ctrl_object->getQuaternion();
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        instance->ctrl_object->setPosition(glm::vec3(pos.x + deltaTime * 2, pos.y, pos.z));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        instance->ctrl_object->setPosition(glm::vec3(pos.x - deltaTime * 2, pos.y, pos.z));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        instance->ctrl_object->setPosition(glm::vec3(pos.x, pos.y, pos.z + deltaTime * 2));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        instance->ctrl_object->setPosition(glm::vec3(pos.x, pos.y, pos.z - deltaTime * 2));
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        instance->ctrl_object->setScale(scale + deltaTime / 2);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        instance->ctrl_object->setScale(scale - deltaTime / 2);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        instance->ctrl_object->setQuaternion(Quat(quat.x, quat.y, quat.z, quat.w + deltaTime * 2));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        instance->ctrl_object->setQuaternion(Quat(quat.x, quat.y, quat.z, quat.w - deltaTime * 2));
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

    instance->scene->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    instance->scene->camera.ProcessMouseScroll(yoffset);
}