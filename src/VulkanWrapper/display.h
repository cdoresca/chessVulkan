#pragma once
#include "vkContext.h"

struct VkDisplay{
	VkSwapchainKHR swapChain;
	VkFormat format;
	VkExtent2D extent;
	VkRenderPass renderPass;
	std::vector<VkImage> image;
	std::vector<VkImageLayout> layout;
	std::vector<VkImageView> view;
	std::vector<VkFramebuffer> frameBuffer;
};

class vulkanDisplayBuilder:public IBuilder<VkDisplay>
{
	VkDisplay display;
	VkContext& context;
	GLFWwindow* window;

	void createSwapChain();
	void createImageViews();
	void createFramebuffers();
	void createRenderPass();

	public:
		vulkanDisplayBuilder(VkContext ctx, GLFWwindow* window);
		void reset() override;
		void build() override;
		VkDisplay get() override;
};
