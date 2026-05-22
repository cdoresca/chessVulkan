#pragma once


#define MAX_FRAMES_IN_FLIGHT  2
#define  MAX_MESHES 1024

#include"texture.h"
/**
 * @brief 
 * La class descriptor gère le VkDescriptorPool, VkSetLayout, 
 * VkDescriptorSet et VkPushConstantRange. Il permet d'ajouter 
 * des VkDescriptorSetLayout et VkPushConstantRange puis il suffit d'appeler la fonction qui 
 * va créer le VkDescriptorPool et les VkDescriptorSet vont se creer automatiquement.
 * 
 */
class descriptor{

	VkDescriptorPool pool;
	VkContext context;


	void cleanup();

	public:
		descriptor(VkContext context);
		~descriptor();

		VkDescriptorSet getSet(uint32_t index,uint32_t offset = 0) const;

		std::vector<VkDescriptorSet> descriptorSet;
		std::vector<VkDescriptorSetLayout> setLayout;
		std::vector<VkPushConstantRange> pushConstant;

		
		void createDescriptorPool(std::vector<VkDescriptorPoolSize> poolSizes);
		void addDescriptorSetLayout(std::vector<VkDescriptorType> type, std::vector<VkShaderStageFlags> flags);
		void addDescriptorSetLayoutBindless(std::vector<VkDescriptorType> type, std::vector<VkShaderStageFlags> flags, uint32_t sizeDescriptor);
		void allocateDescriptorSets();
		void addPushConstant(VkShaderStageFlags flag, uint32_t size);
		void updateTexture(Texture texel);
		
};

