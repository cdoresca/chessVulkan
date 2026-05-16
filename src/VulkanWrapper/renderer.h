#pragma once
#include "pipeline.h"
#include "buffer.h"

class renderer {

	const VkContext& context;
	const VkDisplay& display;
	const VkPipelineDescription& pipeline;
	VkCommand cmd;
	descriptor* m_desc;
	
	void cleanup();

	public:
		
		renderer();
		~renderer();

		Buffer uploadVertex(const std::vector<Vertex> vertices);
		Texture uploadTexture(const char*);
		
};