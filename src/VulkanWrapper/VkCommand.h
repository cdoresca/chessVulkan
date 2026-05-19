#pragma once

#include"vkContext.h"

class VkCommand
{
    VkCommandPool pool;

    VkContext context;
    void createCmdPool();
    void cleanup();
    public:
        VkCommand(VkContext ctx);
        ~VkCommand();
        VkCommandBuffer createCmdBuffer() const;
        void submitCmdBuffer(VkCommandBuffer cmd, bool free = true) const;
        VkCommandPool getPool() const;
};


void copyBuffer(VkBuffer src, VkBuffer des,VkCommandBuffer cmd, VkDeviceSize size);
void copyBufferToImage(VkBuffer buffer, VkImage texel, VkCommandBuffer cmd, uint32_t texWidth, uint32_t texHeight);