#ifndef GAME_H
#define GAME_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"

#include <iostream>

class Game{
public:
    GLFWwindow* window;//����
    Scene* scene; //����
    static Game* Instance();//����ģʽ
    void gameloop();//��Ϸѭ��
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
    const unsigned int src_width;//���ڳ�
    const unsigned int src_height;//���ڿ�
    // timing
    static float deltaTime;
    static float lastFrame;

    static float lastX;
    static float lastY;
    static bool firstMouse;

	//��ʼ��GLFW����
    void initWindow();
	//�û��Զ��庯��
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