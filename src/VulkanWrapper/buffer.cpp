#include"buffer.h"
#include <cstring>
#include "VkCommand.h"

uint32_t findMemoryType(const VkContext& context,uint32_t typeFilter, VkMemoryPropertyFlags properties){
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(context.physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void createBuffer(const VkContext& context, const VkDeviceSize size, const VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Buffer& buffer){

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_CHECK(vkCreateBuffer(context.device, &bufferInfo, nullptr, &buffer.buffer));
		
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(context.device, buffer.buffer, &memRequirements);

	VkMemoryAllocateFlagsInfo allocFlags{};
	allocFlags.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	allocFlags.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(context, memRequirements.memoryTypeBits, properties);
	allocInfo.pNext = &allocFlags;

	VK_CHECK(vkAllocateMemory(context.device, &allocInfo, nullptr, &buffer.memory));
	VK_CHECK(vkBindBufferMemory(context.device, buffer.buffer, buffer.memory, 0));
}

void staggingBuffer(const VkContext& context,const VkDeviceSize size, const VkBufferUsageFlags usage, VkCommand* cmd, Buffer& buffer, void* data){

	Buffer stagging;

	createBuffer( 
        context,
		size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT|usage,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagging
	);

	
	VK_CHECK(vkMapMemory(context.device, stagging.memory, 0, size, 0, &buffer.data));
	memcpy(buffer.data, data, size);
	vkUnmapMemory(context.device, stagging.memory);
	
	VkCommandBuffer cmdBuffer = cmd->createCmdBuffer();
	copyBuffer(stagging.buffer, buffer.buffer, cmdBuffer, size);
	cmd->submitCmdBuffer(cmdBuffer);
	
	vkDestroyBuffer(context.device, stagging.buffer, nullptr);
	vkFreeMemory(context.device, stagging.memory, nullptr);

}

Buffer createBuffer(const VkContext& context,const VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties){
    Buffer tmp;
	tmp.size = size;

	createBuffer(context, size, usage,  properties, tmp);
	return tmp;
}

Buffer createVertexBuffer(const VkContext& context, VkCommand* cmd,const std::vector<Vertex>& vertices){
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | 
		VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	Buffer tmp = createBuffer(context, bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	staggingBuffer(context, bufferSize, usage, cmd, tmp,(void*)vertices.data());
	return tmp;
}

Buffer createIndexBuffer(const VkContext& context, VkCommand* cmd,const std::vector<uint32_t>& index)
{
    VkDeviceSize bufferSize = sizeof(index[0]) * index.size();
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT|
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

	Buffer tmp = createBuffer(context, bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	staggingBuffer(context, bufferSize, usage, cmd, tmp,(void*)index.data());
	return tmp;
}

VkDeviceAddress queryBufferAddress(VkContext& context, VkBuffer other){
    
	VkBufferDeviceAddressInfo info{};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	info.buffer = other;

	return vkGetBufferDeviceAddress(context.device, &info);

}


void destroyBuffer(VkContext ctx, Buffer buffer){
	vkDestroyBuffer(ctx.device,buffer.buffer,nullptr);
	vkFreeMemory(ctx.device, buffer.memory, nullptr);
}

void updateSetUniformBuffer(const VkContext& ctx,VkDescriptorSet set, Buffer buffer)
{
	VkDescriptorBufferInfo descriptorBuffer{};
	descriptorBuffer.buffer = buffer.buffer;
	descriptorBuffer.offset = 0;
	descriptorBuffer.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = nullptr;
	write.dstSet = set;
	write.dstBinding = 0;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.pBufferInfo = &descriptorBuffer;

	vkUpdateDescriptorSets(ctx.device, 1, &write, 0, VK_NULL_HANDLE);
}

Buffer createUnifomBuffer(const VkContext& context, VkCommand* cmd, VkDeviceSize size) {
	Buffer b = createBuffer(context,size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkMapMemory(context.device, b.memory, 0, size, 0, &b.data);

	return b;
}