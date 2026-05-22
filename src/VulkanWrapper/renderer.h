#pragma once
#include "pipeline.h"
#include "buffer.h"

class renderer {

	VkContext context;
	VkCommand* cmd;

	public:
		
	
		renderer(const VkContext& context,VkCommand* cmd);
		~renderer();

		Buffer uploadVertex(const std::vector<Vertex>& vertices);
		Texture uploadTexture(const char*);
		Buffer uploadMatrix(VkDeviceSize size);

		void updateSetUBuffer(VkDescriptorSet set, Buffer matrixBuffer);
		void updateTexture(VkDescriptorSet set, Texture texel);
};