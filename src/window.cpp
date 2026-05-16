#include"window.h"

window::window(){
    glfwInit();
    m_window = glfwCreateWindow(800, 600, "Chess", NULL, NULL);

}

GLFWwindow* window::getGLFW(){ return m_window; }

bool window::isOpen(){ return !glfwWindowShouldClose(m_window); }