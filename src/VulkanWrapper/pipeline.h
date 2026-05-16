#pragma once
#include "display.h"
#include "shader.h"
#include "descriptor.h"



struct VkPipelineDescription{

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

};

class vulkanPipelineBuilder: public IBuilder<VkPipelineDescription>{

	const VkContext& ctx;
	const VkDisplay& display;
	VkPipelineDescription pipeline;

	descriptor* m_descriptor;

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	void createPipelineLayout();
	void createPipeline();
	
	public:
	vulkanPipelineBuilder(const VkContext& ctx, const VkDisplay& display, descriptor* d);
	
		void addShader(shader obj);
		void reset() override;
		void build() override;
		VkPipelineDescription get() override;
};

