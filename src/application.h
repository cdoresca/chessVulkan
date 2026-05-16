#pragma once
#include "game.h"

#include "VulkanWrapper/command.h"
#include "VulkanWrapper/pipeline.h"
#include "camera.h"


class app{

    VkContext ctx;
    VkDisplay display;
    GLFWwindow* window;
    VkCommand* cmd;
    descriptor* m_descriptor;
    VkPipelineDescription pipeline;
    Camera camera;

    game chess;

    void build();
    public:
        app();

};