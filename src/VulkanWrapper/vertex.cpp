#include"vertex.h"
#include<vulkan/vulkan.h>

VkVertexInputBindingDescription getBindingDescription(){
	VkVertexInputBindingDescription binding{};
	binding.binding = 0;
	binding.stride = sizeof(Vertex);
	binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return binding;
}

std::array<VkVertexInputAttributeDescription,3> getAttributeDescription(){
	std::array<VkVertexInputAttributeDescription,3> attribute;
	

	attribute[0].binding = 0;
	attribute[0].location = 0;
	attribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[0].offset = offsetof(Vertex,Vertex::position);

	attribute[1].binding = 0;
	attribute[1].location = 1;
	attribute[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[1].offset = offsetof(Vertex,Vertex::normal);

	attribute[2].binding = 0;
	attribute[2].location = 0;
	attribute[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[2].offset = offsetof(Vertex,Vertex::texcoord);

	return attribute;
}