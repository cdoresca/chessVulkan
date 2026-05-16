#pragma once

#include"vkContext.h"

class VkCommand
{
    VkCommandPool pool;

    VkContext context;

    public:
        VkCommand(VkContext ctx);
        VkCommandBuffer createCmdBuffer();
        void submitCmdBuffer(VkCommandBuffer cmd, bool free = true);
};


void copyBuffer(VkBuffer src, VkBuffer des,VkCommand cmd, VkDeviceSize size);
void copyBufferToImage(VkBuffer buffer, VkImage texel, VkCommandBuffer cmd, uint32_t texWidth, uint32_t texHeight);