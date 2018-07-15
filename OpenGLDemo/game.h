#ifndef GAME_H
#define GAME_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"

#include <iostream>

class Game{
public:
    GLFWwindow* window;//窗口
    Scene* scene; //场景
    static Game* Instance();//单例模式
    void gameloop();//游戏循环
    void setCtrlModel(Model* go);
private:
	Game();
    ~Game(){
        glfwTerminate();
        delete(scene);
        delete(window);
        delete(instance);
    }

    static Game *instance;

    static Model* ctrl_object;
    const unsigned int src_width;//窗口长
    const unsigned int src_height;//窗口宽
    // timing
    static float deltaTime;
    static float lastFrame;

    static float lastX;
    static float lastY;
    static bool firstMouse;

	//初始化GLFW窗口
    void initWindow();
	//用户自定义函数
	void initWindowEvent();
	void start();
	void update();

    static bool instanceCheck();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void processInput(GLFWwindow *window);
	static void ModelCtrl(GLFWwindow *window);
};

#endif