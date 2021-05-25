#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


float vertices[] = {
        -0.5f, -0.5, 0.0f,
        0.5f, -0.5, 0.0f,
        0.0f, 0.5f, 0.0f,
};


/**
 * 当用户改变窗口大小时的回调函数
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/**
 * 处理用户输入
 * @param window
 */
void processInput(GLFWwindow *window);

int main() {

    //初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    //注册窗口回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    while (!glfwWindowShouldClose(window)) {
        //输入
        processInput(window);

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        //检查并调用事件,交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    glfwTerminate();

    return 0;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}