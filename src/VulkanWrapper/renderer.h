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
		
};