#pragma once
#include<GLFW/glfw3.h>


class window{

    GLFWwindow* m_window;

    public:
        window();
        GLFWwindow* getGLFW();
        bool isOpen();
    
};