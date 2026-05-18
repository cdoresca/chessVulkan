#include"application.h"
#include "VulkanWrapper/IBuilder.h"
#include "VulkanWrapper/shader.h"
#include "UI/cellUI.h"

app::app(){
    build();
}

app::~app(){ cleanup();}

void app::build(){

    glfwInit();
    m_window = new window();

    camera = buildCamera(glm::vec3(0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f, 1.0f,0.0f));

    IBuilder<VkContext>* ctxBuilder = new vulkanContextBuilder(m_window->getGLFW());
    director<VkContext> dir(ctxBuilder);
    dir.make();
    ctx = ctxBuilder->get();

    IBuilder<VkDisplay>* displayBuilder = new vulkanDisplayBuilder(ctx, m_window->getGLFW());
    director<VkDisplay> dirDisplay(displayBuilder);
    dirDisplay.make();
    display = displayBuilder->get();

    //shader
    shader vertexShader(ctx,"shader/vert.glsl",VK_SHADER_STAGE_VERTEX_BIT);
    shader fragmentShader(ctx,"shader/frag.glsl",VK_SHADER_STAGE_FRAGMENT_BIT);

    //descriptor
    m_descriptor = new descriptor(ctx);
	m_descriptor->addPushConstant(VK_SHADER_STAGE_VERTEX_BIT,sizeof(Camera)); 

    //command
    cmd = new VkCommand(ctx);

    //pipeline
    IBuilder<VkPipelineDescription>* pipelineBuilder = new vulkanPipelineBuilder(ctx,display,m_descriptor,{vertexShader,fragmentShader});
	director<VkPipelineDescription> dirPipeline(pipelineBuilder);
	dirPipeline.make();
	pipeline = pipelineBuilder->get();

	m_render = new renderer(ctx,cmd);
    createSyncObjects();
    createCmdBuffer();

    currentFrame = 0;

	delete ctxBuilder;
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

    beginRecordCommand(commands[imageIndex], imageIndex);
    render(commands[imageIndex]);
    endRecordCommand(commands[imageIndex], imageIndex);

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commands[currentFrame];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[imageIndex] };
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

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
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

void app::render(VkCommandBuffer cmd){
    for(int i = 0; i < models.size(); i++){
        models[i]->render(cmd);
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

void app::endRecordCommand(VkCommandBuffer cmd, uint32_t index){
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
	//models.push_back(new cellUI(m_render));
}

void app::run(){
	while (m_window->isOpen())
	{
		glfwPollEvents();
		drawFrame();
	}
	
}

void app::recreateSwapChain(){
	IBuilder<VkDisplay>* displayBuilder = new vulkanDisplayBuilder(ctx, m_window->getGLFW());
    director<VkDisplay> dirDisplay(displayBuilder);
    dirDisplay.make();
    display = displayBuilder->get();

	delete displayBuilder;

}