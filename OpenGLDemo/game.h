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
    void gameloop();//游戏循环
    static Game* Instance();//单例模式
    void setCtrlModel(GameObject* go);
private:
    Game():src_width(1280),src_height(720){
        deltaTime = 0.0f;
        lastFrame = 0.0f;
        firstMouse = true;
        float lastX = (float)src_width / 2.0;
        float lastY = (float)src_height / 2.0;
        scene = new Scene();
        window = NULL;
        ctrl_object = NULL;
        initWindow();
    }
    ~Game(){
        delete(scene);
        delete(window);
        delete(instance);
    }
    static Game *instance;

    static GameObject* ctrl_object;
    const unsigned int src_width;//窗口长
    const unsigned int src_height;//窗口宽
    // timing
    static float deltaTime;
    static float lastFrame;

    static float lastX;
    static float lastY;
    static bool firstMouse;

    void initWindow();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow *window);
    static void ModelCtrl(GLFWwindow *window);

    static bool instanceCheck();
};

#endif