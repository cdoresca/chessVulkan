#include"application.h"
#include "VulkanWrapper/IBuilder.h"
#include "VulkanWrapper/shader.h"
#include "UI/cellUI.h"
#include <VulkanWrapper/physicalDeviceInformation.h>

app::app(){
    build();
}

app::~app(){ cleanup();}

void app::build(){

    
    m_window = new window();

    camera = buildCamera(glm::vec3(0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f, 1.0f,0.0f));

	
	buildDevice();

	vulkanDisplayBuilder* displayBuilder = new vulkanDisplayBuilder(ctx, m_window->getGLFW());
    director<VkDisplay> dirDisplay(displayBuilder);
    dirDisplay.make();
    display = displayBuilder->get();

    //shader
    shader vertexShader(ctx,"shader/vert.glsl.spv",VK_SHADER_STAGE_VERTEX_BIT);
    shader fragmentShader(ctx, "shader/frag.glsl.spv",VK_SHADER_STAGE_FRAGMENT_BIT);

    //descriptor
    m_descriptor = new descriptor(ctx);
	m_descriptor->addPushConstant(VK_SHADER_STAGE_VERTEX_BIT,sizeof(Camera));

	std::vector<VkDescriptorType> typeSet = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };
	std::vector<VkShaderStageFlags> flagSet = { VK_SHADER_STAGE_VERTEX_BIT };
	m_descriptor->addDescriptorSetLayout(typeSet, flagSet);

	std::vector<VkDescriptorType> typeSet1 = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER };
	std::vector<VkShaderStageFlags> flagSet1 = { VK_SHADER_STAGE_FRAGMENT_BIT };
	m_descriptor->addDescriptorSetLayout(typeSet1, flagSet1);

	std::vector<VkDescriptorPoolSize> poolSizes = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT },
	};
	m_descriptor->createDescriptorPool(poolSizes);
	m_descriptor->allocateDescriptorSets();

    //command
    cmd = new VkCommand(ctx);
	
    //pipeline
	vulkanPipelineBuilder* pipelineBuilder = new vulkanPipelineBuilder(ctx,display,m_descriptor);
	pipelineBuilder->addShader(vertexShader);
	pipelineBuilder->addShader(fragmentShader);
	director<VkPipelineDescription> dirPipeline(pipelineBuilder);	
	dirPipeline.make();
	pipeline = pipelineBuilder->get();

	m_render = new renderer(ctx,cmd);
    createSyncObjects();
    createCmdBuffer();

    currentFrame = 0;

	addModels();


	delete displayBuilder;
	delete pipelineBuilder;
    
}

void app::drawFrame(){

    VK_CHECK(vkWaitForFences(ctx.device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX));
    VK_CHECK(vkResetFences(ctx.device, 1, &inFlightFences[currentFrame]));

	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(
		ctx.device,
		display.swapChain,
		UINT64_MAX,
		imageAvailableSemaphores[currentFrame],
		VK_NULL_HANDLE,
		&imageIndex
	);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	VK_CHECK(vkResetCommandBuffer(commands[currentFrame], 0));

    beginRecordCommand(commands[currentFrame], imageIndex);
    render(commands[currentFrame],currentFrame);
    endRecordCommand(commands[currentFrame]);

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[imageIndex] };

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commands[currentFrame];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VK_CHECK(vkQueueSubmit(ctx.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]));

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { display.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(ctx.presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window->framebufferResized) {
		m_window->framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	return;
}

void app::createSyncObjects(){
    uint32_t imageCount = display.image.size();

	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(imageCount);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VK_CHECK(vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]));
		VK_CHECK(vkCreateFence(ctx.device, &fenceInfo, nullptr, &inFlightFences[i]));
	}
	for (size_t i = 0; i < imageCount; i++) {
		VK_CHECK(vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]));
	}
}

void app::createCmdBuffer(){

    commands.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0;i < commands.size(); i++){
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = cmd->getPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
	
	VK_CHECK(vkAllocateCommandBuffers(ctx.device, &allocInfo, &commands[i]));
    }
}

void app::render(VkCommandBuffer cmd, uint32_t frameIndex){
    for(int i = 0; i < models.size(); i++){
        models[i]->render(cmd, frameIndex,m_descriptor);
    }
}

void app::beginRecordCommand(VkCommandBuffer cmd, uint32_t imageIndex){

    VkCommandBufferBeginInfo beginInfo {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = display.renderPass;
	renderPassInfo.framebuffer = display.frameBuffer[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = display.extent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)display.extent.width;
	viewport.height = (float)display.extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(cmd, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = display.extent;
	vkCmdSetScissor(cmd, 0, 1, &scissor);

}

void app::endRecordCommand(VkCommandBuffer cmd){
    vkCmdEndRenderPass(cmd);
	VK_CHECK(vkEndCommandBuffer(cmd));

}

void app::cleanup(){
	for(int i = 0; i < inFlightFences.size(); i++)
		vkDestroyFence(ctx.device,inFlightFences[i],nullptr);
	for(int i = 0; i < imageAvailableSemaphores.size(); i++)
		vkDestroySemaphore(ctx.device,imageAvailableSemaphores[i],nullptr);
	for(int i = 0; i < renderFinishedSemaphores.size(); i++)
		vkDestroySemaphore(ctx.device,renderFinishedSemaphores[i],nullptr);
	for(int i = 0; i < models.size(); i++)
		delete models[i];
	
	delete cmd;
	delete m_descriptor;

	destroyVkPipelineDescription(ctx,pipeline);
	destroyVkDisplay(ctx,display);
	destroyVkContext(ctx);
	
}

void app::addModels(){
	models.push_back(new cellUI(m_render));
}

void app::run(){
	while (m_window->isOpen())
	{
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(ctx.device);
	
}

void app::recreateSwapChain(){
	IBuilder<VkDisplay>* displayBuilder = new vulkanDisplayBuilder(ctx, m_window->getGLFW());
    director<VkDisplay> dirDisplay(displayBuilder);
    dirDisplay.make();
    display = displayBuilder->get();

	delete displayBuilder;

}

void app::buildDevice() {

	std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
	};

	std::vector<PhysicalDeviceFeature> featureBasic = {
		{ "geometryShader", &VkPhysicalDeviceFeatures::geometryShader },
		{ "samplerAnisotropy", &VkPhysicalDeviceFeatures::samplerAnisotropy }
	};

	VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures {};
	bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
	bufferAddressFeatures.bufferDeviceAddress = VK_TRUE;
	
	VkPhysicalDeviceFeatures2 feature{};
	feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

	add(feature, reinterpret_cast<VkBaseOutStructure*>(&bufferAddressFeatures));

	vulkanContextBuilder* ctxBuilder = new vulkanContextBuilder(m_window->getGLFW(), featureBasic, deviceExtensions ,feature);
	director<VkContext> dir(ctxBuilder);
	dir.make();
	ctx = ctxBuilder->get();

	delete ctxBuilder;

}