#include "display.h"
#include "swapChainSupportDetail.h"
#include "queueFamily.h"


void vulkanDisplayBuilder::createSwapChain() {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(context.physicalDevice, context.surface);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentsModes);

	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

	uint32_t minImageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 &&
		minImageCount > swapChainSupport.capabilities.maxImageCount) {
		minImageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = context.surface;
	createInfo.minImageCount = minImageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	;

	QueueFamiliesIndices indices = findQueueFamilies(context.physicalDevice, context.surface);
	uint32_t queueFamilyIndices[] = { indices.graphicFamily.value(), indices.presentFamily.value() };
	if (indices.graphicFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CHECK(vkCreateSwapchainKHR(context.device, &createInfo, nullptr, &display.swapChain));

	uint32_t imageCount;
	VK_CHECK(vkGetSwapchainImagesKHR(context.device, display.swapChain, &imageCount, nullptr));
	display.image.resize(imageCount);

	VK_CHECK(vkGetSwapchainImagesKHR(context.device, display.swapChain, &imageCount, display.image.data()));

	display.format = surfaceFormat.format;
	display.extent = extent;

	display.layout.resize(imageCount);

	for (size_t i = 0; i < display.layout.size(); i++)
	{
		display.layout[i] = VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

void  vulkanDisplayBuilder::createImageViews() {
	display.view.resize(display.view.size());

	for (size_t i = 0; i < display.image.size(); i++) {

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = display.image[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = display.format;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(context.device, &createInfo, nullptr, &display.view[i]));
	}
}

void vulkanDisplayBuilder::createFramebuffers() {
	display.frameBuffer.resize(display.view.size());

	for (size_t i = 0; i < display.view.size(); i++) {
		VkImageView attachments[] = {
			display.view[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = display.renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = display.extent.width;
		framebufferInfo.height = display.extent.height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(context.device, &framebufferInfo, nullptr, &display.frameBuffer[i]));
	}
}
void createAttachementDescription(VkFormat format, VkAttachmentDescription& colorAttachment){
	colorAttachment.format = format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

}

void createAttachementReference(VkAttachmentReference& colorAttachmentRef){
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}
void createSubpassDescription(VkSubpassDescription& subpass, VkAttachmentReference& colorAttachmentRef){
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
}
void createSubpassDependency(VkSubpassDependency& dependency) {
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
	dependency.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
								VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
}
void vulkanDisplayBuilder::createRenderPass() {

	VkAttachmentDescription colorAttachment{};
	createAttachementDescription(display.format, colorAttachment);

	VkAttachmentReference colorAttachmentRef{};
	createAttachementReference(colorAttachmentRef);

	VkSubpassDescription subpass{};
	createSubpassDescription(subpass, colorAttachmentRef);

	VkSubpassDependency dependency{};
	createSubpassDependency(dependency);

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_CHECK(vkCreateRenderPass(context.device, &renderPassInfo, nullptr, &display.renderPass));

}

void vulkanDisplayBuilder::reset(){ display = {};}

void vulkanDisplayBuilder::build() {
	createSwapChain();
	createImageViews();
	createRenderPass();
	createFramebuffers();
}
VkDisplay vulkanDisplayBuilder::get(){ return display; }


vulkanDisplayBuilder::vulkanDisplayBuilder(VkContext ctx,GLFWwindow* window):context(ctx),window(window){}