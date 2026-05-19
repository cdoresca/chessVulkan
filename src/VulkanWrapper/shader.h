#pragma once

#include <vector>

#include"vkContext.h"



class shader {



 	VkContext ctx;

	VkShaderModule shaderModule;

	VkPipelineShaderStageCreateInfo pipelineShader;



	VkShaderModule createShaderModule(const std::vector<char>& code);

 	VkPipelineShaderStageCreateInfo createPipelineShaderStageInfo(const char* file, VkShaderStageFlagBits stage);

	void cleanup();



	public:

	shader(const VkContext& context,const char* path, VkShaderStageFlagBits stage);
	
	~shader();

	void debug() const {
		std::cout << "shaderModule = " << shaderModule << std::endl;
	}

		VkPipelineShaderStageCreateInfo getPipelineShader()const;

};



std::vector<char> readFile(const char* filename);