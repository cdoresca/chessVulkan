#pragma once
#include "vulkan/vulkan.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>

#include"IBuilder.h"
#include<GLFW/glfw3.h>
#include "physicalDeviceInformation.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)


#define DEFAULT_FENCE_TIMEOUT 100000000000

std::vector<const char*> getRequiredExtensions();

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"

};

const std::vector<VkValidationFeatureEnableEXT> enables = {
	VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
	VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger
);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator
);

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
);

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

struct RayTracingProcs {
	PFN_vkCreateAccelerationStructureKHR        vkCreateAccelerationStructureKHR = nullptr;
	PFN_vkDestroyAccelerationStructureKHR       vkDestroyAccelerationStructureKHR = nullptr;
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = nullptr;
	PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = nullptr;
	PFN_vkCmdBuildAccelerationStructuresKHR     vkCmdBuildAccelerationStructuresKHR = nullptr;
	PFN_vkCreateRayTracingPipelinesKHR          vkCreateRayTracingPipelinesKHR = nullptr;
	PFN_vkGetRayTracingShaderGroupHandlesKHR    vkGetRayTracingShaderGroupHandlesKHR = nullptr;
	PFN_vkCmdTraceRaysKHR                       vkCmdTraceRaysKHR = nullptr;
};

struct VkContext
{
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;

    VkDebugUtilsMessengerEXT debugMessenger;
};

// patron builder
class vulkanContextBuilder: public IBuilder<VkContext>
{
    VkContext context;
    GLFWwindow* window;

    VkPhysicalDeviceFeatures2 features2;

    std::vector<const char*> deviceExtensions;
    std::vector<PhysicalDeviceFeature> featureBasic;

    float priorityQueue = 1.0f;

    void createInfoQueue(std::vector<VkDeviceQueueCreateInfo>& info);
    
    bool isDeviceSuitable(const VkPhysicalDevice gpu);
    bool isFeatureSuitable(VkPhysicalDeviceFeatures features) const;
    bool isQueueSuitable(const VkPhysicalDevice gpu) const;
    bool isSwapChainSuitable(const VkPhysicalDevice gpu) const;
    bool isExtensionSuitable(const VkPhysicalDevice gpu) const;
    bool isModernFeatureSuitable(const VkPhysicalDevice gpu);
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void choosePhysicalDevice();
    void createDevice();
    void createQueue();
    
    public:
    
    
        vulkanContextBuilder(GLFWwindow* window,const std::vector<PhysicalDeviceFeature>& featureBasic, std::vector<const char*> deviceExtensions, VkPhysicalDeviceFeatures2 features2);
        
        void reset() override;
        void build() override;

        VkContext get() override;
};

void destroyVkContext(VkContext ctx);