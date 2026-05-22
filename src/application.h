#pragma once
#include "game.h"

#include "VulkanWrapper/pipeline.h"
#include "VulkanWrapper/model.h"
#include "camera.h"
#include "window.h"


class app{

    VkContext ctx;
    VkDisplay display;
    window* m_window;
    VkCommand* cmd;
    descriptor* m_descriptor;
    VkPipelineDescription pipeline;
    Camera camera;
    renderer* m_render;
    
    game chess;

    std::vector<model*> models;
    std::vector<VkFence> inFlightFences;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkCommandBuffer> commands;

    uint32_t currentFrame;

  
    void buildDevice();
    void build();
    void drawFrame();
    void render(VkCommandBuffer cmd);
    void createSyncObjects();
    void createCmdBuffer();
    void beginRecordCommand(VkCommandBuffer cmd, uint32_t imageIndex);
    void endRecordCommand(VkCommandBuffer cmd);
    void recreateSwapChain();
    void cleanup();
    void addModels();

    public:
        app();
        ~app();
        void run();

};