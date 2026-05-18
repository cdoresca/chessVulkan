#include "renderer.h"

renderer::renderer(const VkContext& context,VkCommand* cmd):context(context),cmd(cmd){}

Buffer renderer::uploadVertex(const std::vector<Vertex>& vertices){ return createVertexBuffer(context, cmd,  vertices); }

Texture renderer::uploadTexture(const char* path){ return createTexture(context,path, cmd); }