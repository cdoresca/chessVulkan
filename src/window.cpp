#include"window.h"

window::window(){
    glfwInit();
    m_window = glfwCreateWindow(800, 600, "Chess", NULL, NULL);
    glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

GLFWwindow* window::getGLFW(){ return m_window; }

bool window::isOpen(){ return !glfwWindowShouldClose(m_window); }

void window::framebufferResizeCallback(GLFWwindow* glwindow, int width, int height){
    auto app = reinterpret_cast<window*>(glfwGetWindowUserPointer(glwindow));
	app->framebufferResized = true;
}

void window::cleanup(){ glfwDestroyWindow(m_window);}