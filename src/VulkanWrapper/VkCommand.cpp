#include "VkCommand.h"

VkCommand::VkCommand(VkContext ctx):context(ctx){}

VkCommandBuffer createTmpCommandBuffer(const VkContext& context,VkCommandPool pool,VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, VkCommandBufferInheritanceInfo* info = nullptr){
    VkCommandBuffer tmp;

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;
	
	VK_CHECK(vkAllocateCommandBuffers(context.device, &allocInfo, &tmp));

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = flags;
	beginInfo.pInheritanceInfo = info;

	VK_CHECK(vkBeginCommandBuffer(tmp, &beginInfo));
	
	return tmp;

}

// bool free : true if commandbuffer is one time submit false if want to reuse it
void flushCommandBuffer(const VkContext& context,VkCommandPool pool,VkCommandBuffer commandBuffer, bool free = true){
    if (commandBuffer == VK_NULL_HANDLE)
	{
		return;
	}

	VK_CHECK(vkEndCommandBuffer(commandBuffer));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;



	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	VkFence fence;

	VK_CHECK(vkCreateFence(context.device, &fenceInfo, nullptr, &fence));
	VK_CHECK(vkQueueSubmit(context.graphicsQueue, 1, &submitInfo, fence));
	VK_CHECK(vkWaitForFences(context.device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));
	vkDestroyFence(context.device, fence, nullptr);
	if (free)
	{
		vkFreeCommandBuffers(context.device, pool, 1, &commandBuffer);
	}
}

VkCommandBuffer VkCommand::createCmdBuffer() const { return createTmpCommandBuffer(context, pool); }

void VkCommand::submitCmdBuffer(VkCommandBuffer cmd, bool free) const{
	flushCommandBuffer(context, pool, cmd, free);
}

void copyBuffer(VkBuffer src, VkBuffer dst,VkCommandBuffer cmd, VkDeviceSize size){

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; 
	copyRegion.dstOffset = 0; 
	copyRegion.size = size;

	vkCmdCopyBuffer(cmd, src, dst, 1, &copyRegion);
}

void copyBufferToImage(VkBuffer buffer, VkImage texel, VkCommandBuffer cmd, uint32_t width, uint32_t height){

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		cmd,
		buffer,
		texel,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);
}

VkCommandPool VkCommand::getPool() const { return pool;}

void VkCommand::cleanup(){
	vkDestroyCommandPool(context.device,pool,nullptr);
}