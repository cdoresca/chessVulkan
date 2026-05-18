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
	void addShader(shader obj);
	
	public:
		vulkanPipelineBuilder(const VkContext& ctx, const VkDisplay& display, descriptor* d,std::vector<shader> s);
	
		void reset() override;
		void build() override;
		VkPipelineDescription get() override;
};

void destroyVkPipelineDescription(VkContext ctx, VkPipelineDescription pipeline);