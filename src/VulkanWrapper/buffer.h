#pragma once
#include"vkContext.h"
#include "vertex.h"
#include "descriptor.h"

struct Buffer
{
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkDeviceSize size;
	void* data;
};


Buffer createBuffer(const VkContext& context,const VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
Buffer createVertexBuffer(const VkContext& context, VkCommand* cmd, const std::vector<Vertex>&);
Buffer createUnifomBuffer(const VkContext& context, VkCommand* cmd, VkDeviceSize size);
Buffer createIndexBuffer(const VkContext& context, VkCommand* cmd, const std::vector<uint32_t>&);

VkDeviceAddress queryBufferAddress(VkContext& context, VkBuffer other);

void updateSetUniformBuffer(const VkContext& ctx, VkDescriptorSet set, Buffer buffer);

void destroyBuffer(VkContext ctx, Buffer buffer);