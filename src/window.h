#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>


class window{

    GLFWwindow* m_window;
    
    void cleanup();
    
    public:
        bool framebufferResized;
        window();
        GLFWwindow* getGLFW();
        bool isOpen();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    
};