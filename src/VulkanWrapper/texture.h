#pragma once
#include"vkContext.h"
#include "command.h"

struct Texture {
    
	VkDeviceMemory memory;
	VkImage image;
	VkImageView view;
	VkSampler sample;
    
};

Texture createTexture(const VkContext context, const char* path, VkCommand cmd);


