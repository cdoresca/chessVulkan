#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vulkan/vulkan.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
	glm::vec2 texcoord;
};

VkVertexInputBindingDescription getBindingDescription();
std::array<VkVertexInputAttributeDescription,3> getAttributeDescription();