#pragma once
#include"vkContext.h"
#include "VkCommand.h"

struct Texture {
    
	VkDeviceMemory memory;
	VkImage image;
	VkImageView view;
	VkSampler sample;
    
};

Texture createTexture(const VkContext context, const char* path, VkCommand* cmd);

void destroyTexture(VkContext ctx,Texture texel);