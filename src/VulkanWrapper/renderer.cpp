#include "renderer.h"

Buffer renderer::uploadVertex(const std::vector<Vertex> vertices){ return createVertexBuffer(context, cmd,  vertices); }

Texture renderer::uploadTexture(const char* path){ return createTexture(context,path, cmd); }