#pragma once
#include"vkContext.h"
#include "vertex.h"

struct Buffer
{
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkDeviceSize size;
	void* data;
};


Buffer createBuffer(const VkContext& context,const VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
Buffer createVertexBuffer(const VkContext& context, VkCommand cmd, const std::vector<Vertex>&);
Buffer createIndexBuffer(const VkContext& context, VkCommand cmd, const std::vector<uint32_t>&);

VkDeviceAddress queryBufferAddress(VkContext& context, VkBuffer other);