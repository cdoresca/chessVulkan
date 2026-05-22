#include "descriptor.h"

void descriptor::createDescriptorPool(std::vector<VkDescriptorPoolSize> poolSizes)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descriptorPoolInfo.pPoolSizes = poolSizes.data();
	descriptorPoolInfo.maxSets = setLayout.size() * MAX_FRAMES_IN_FLIGHT;
	descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

	VK_CHECK(vkCreateDescriptorPool(context.device, &descriptorPoolInfo, nullptr, &pool));
}

void descriptor::addDescriptorSetLayout(std::vector<VkDescriptorType> type, std::vector<VkShaderStageFlags> flags)
{
	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;

	for (int i = 0; i < type.size(); i++) {

		VkDescriptorSetLayoutBinding setLayoutBinding{};
		setLayoutBinding.binding = i;
		setLayoutBinding.descriptorType = type[i];
		setLayoutBinding.descriptorCount = 1;
		setLayoutBinding.stageFlags = flags[i];

		setLayoutBindings.push_back(setLayoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetlayoutInfo{};
	descriptorSetlayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetlayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
	descriptorSetlayoutInfo.pBindings = setLayoutBindings.data();

	VkDescriptorSetLayout layout;
	VK_CHECK(vkCreateDescriptorSetLayout(context.device, &descriptorSetlayoutInfo, nullptr,&layout));
	setLayout.push_back(layout);

}

void descriptor::addDescriptorSetLayoutBindless(std::vector<VkDescriptorType> type, std::vector<VkShaderStageFlags> flags,  uint32_t sizeDescriptor)
{

	std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
	std::vector<VkDescriptorBindingFlags> bindingFlags;

	for (int i = 0; i < type.size(); i++) {

		VkDescriptorSetLayoutBinding setLayoutBinding{};
		setLayoutBinding.binding = i;
		setLayoutBinding.descriptorType = type[i];
		setLayoutBinding.descriptorCount = sizeDescriptor;
		setLayoutBinding.stageFlags = flags[i];

		setLayoutBindings.push_back(setLayoutBinding);
		bindingFlags.push_back(
			VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
			VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
		);
	}

	VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo{};
	flagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	flagsInfo.bindingCount = bindingFlags.size();
	flagsInfo.pBindingFlags = bindingFlags.data();

	VkDescriptorSetLayoutCreateInfo descriptorSetlayoutInfo{};
	descriptorSetlayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetlayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
	descriptorSetlayoutInfo.pBindings = setLayoutBindings.data();
	descriptorSetlayoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT; 

	VkDescriptorSetLayout layout;
	VK_CHECK(vkCreateDescriptorSetLayout(context.device, &descriptorSetlayoutInfo, nullptr, &layout));
	setLayout.push_back(layout);
}

void descriptor::allocateDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts;

	for (int i = 0; i < setLayout.size() * MAX_FRAMES_IN_FLIGHT; i++) {
		layouts.push_back(setLayout[i % setLayout.size()]);
	}

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.pSetLayouts = layouts.data();
	allocInfo.descriptorSetCount = layouts.size();

	
	descriptorSet.resize(layouts.size());
	VK_CHECK(vkAllocateDescriptorSets(context.device, &allocInfo, descriptorSet.data()));
}

void descriptor::addPushConstant(VkShaderStageFlags flag,uint32_t size)
{	
	VkPushConstantRange range{};
	range.stageFlags = flag;
	range.offset = pushConstant.empty() ? 0 : pushConstant.back().size;
	range.size = size;

	pushConstant.push_back(range);
}

void descriptor::cleanup()
{
	for (int i = 0; i < setLayout.size(); i++) {
		vkDestroyDescriptorSetLayout(context.device, setLayout[i], nullptr);
	}

	vkDestroyDescriptorPool(context.device,pool, nullptr);
}

descriptor::descriptor(VkContext context):context(context)
{
}

descriptor::~descriptor()
{
	cleanup();
}



void descriptor::updateTexture(Texture texel){
	VkDescriptorImageInfo imageInfo{};    
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = texel.view;
	imageInfo.sampler = texel.sample;

	VkWriteDescriptorSet writeDescriptor{};

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		
		VkDescriptorSet set = getSet(i);

		VkWriteDescriptorSet writeDescriptor{};
		writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptor.dstSet = set;
		writeDescriptor.dstBinding = 0;
		writeDescriptor.dstArrayElement = 0;
		writeDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptor.descriptorCount = 1;
		writeDescriptor.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(context.device, 1, &writeDescriptor, 0, nullptr);
	}
}

VkDescriptorSet descriptor::getSet(uint32_t index,uint32_t offset) const{
	return descriptorSet[setLayout.size() * index + offset];
}

