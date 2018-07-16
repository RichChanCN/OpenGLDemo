#include "game.h"
Game* Game::instance = NULL;
Model* Game::ctrl_object = NULL;
// timing
float Game::deltaTime = 0.0f;
float Game::lastFrame = 0.0f;

float Game::lastX = 0.0f;
float Game::lastY = 0.0f;
bool Game::firstMouse = true;

Game::Game() :src_width(1280), src_height(720){
	deltaTime = 0.0f;
	lastFrame = 0.0f;
	firstMouse = true;
	float lastX = (float)src_width / 2.0;
	float lastY = (float)src_height / 2.0;
	scene = Scene();
	window = NULL;
	ctrl_object = NULL;
	initWindow();
}

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
void Game::setCtrlModel(Model* go){
    instance->ctrl_object = go;
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
	initWindowEvent();

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
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //------------------------------
	start();
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
		update();
        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.render(src_width, src_height, lastFrame);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    return;
}
