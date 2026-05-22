#pragma once

#include "vulkan/vulkan.h"
#include<vector>

/**
 * @brief 
 * La struct PhysicalDevicePropriete contient les propriétés du processeur graphique.
 */
struct PhysicalDevicePropriete {
	VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_rtProperties{};
	VkPhysicalDeviceAccelerationStructurePropertiesKHR m_asProperties{};
	VkPhysicalDeviceDescriptorIndexingProperties indexingProps{};
	VkPhysicalDeviceProperties2 prop2{};
};

PhysicalDevicePropriete queryphysicalDevicePropriete(VkPhysicalDevice);

void add(VkPhysicalDeviceFeatures2& feature2 , VkBaseOutStructure* feature);
struct PhysicalDeviceFeature
{
	const char* name;
	VkBool32 VkPhysicalDeviceFeatures::* member;
};