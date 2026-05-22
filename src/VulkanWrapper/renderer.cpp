#include "renderer.h"

renderer::renderer(const VkContext& context,VkCommand* cmd):context(context),cmd(cmd){}

Buffer renderer::uploadVertex(const std::vector<Vertex>& vertices){ return createVertexBuffer(context, cmd,  vertices); }

Buffer renderer::uploadMatrix(VkDeviceSize size){ return createUnifomBuffer(context, cmd, size); }

Texture renderer::uploadTexture(const char* path){ return createTexture(context,path, cmd); }

void renderer::updateSetUBuffer(VkDescriptorSet set, Buffer matrixBuffer) {	updateSetUniformBuffer(context, set, matrixBuffer); }

void renderer::updateTexture(VkDescriptorSet set, Texture texel) { updateSetTexture(context, set, texel); }
