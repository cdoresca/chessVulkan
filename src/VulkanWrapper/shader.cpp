#include "shader.h"
//#include <filesystem>

std::vector<char> readFile(const char* filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	//std::cout << std::filesystem::current_path() << std::endl;
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule shader::createShaderModule(const std::vector<char>& code) {

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;

	VK_CHECK(vkCreateShaderModule(ctx.device, &createInfo, nullptr, &shaderModule));

	return shaderModule;
}


VkPipelineShaderStageCreateInfo shader::createPipelineShaderStageInfo(const char* file, VkShaderStageFlagBits stage)
{
	std::vector<char> shaderCode = readFile(file);
	shaderModule = createShaderModule(shaderCode);

	VkPipelineShaderStageCreateInfo pipelineShaderStageInfo{};
	pipelineShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageInfo.stage = stage;
	pipelineShaderStageInfo.module = shaderModule;
	pipelineShaderStageInfo.pName = "main";

	return pipelineShaderStageInfo;
}



void shader::cleanup()
{
	vkDestroyShaderModule(ctx.device, shaderModule, nullptr);
}


shader::shader(const VkContext& context,const char* path, VkShaderStageFlagBits stage):ctx(context)
{
	pipelineShader = createPipelineShaderStageInfo(path, stage);
}


shader::~shader()
{
	cleanup();
}

VkPipelineShaderStageCreateInfo shader::getPipelineShader() const
{
	return pipelineShader;
}
