#include "cellUI.h"

cellUI::cellUI(VkContext ctx, VkPipelineDescription pipeline, VkCommand* command):model(ctx, pipeline, command){

    //texel = createTexture(ctx,IMAGE_DIR"/square brown dark_1x_ns.png",cmd);
}

void cellUI::render(VkCommandBuffer cmd, uint32_t frameIndex,descriptor* desc) const
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &verticesBuffer.buffer,&offset);
    VkDescriptorSet set[] = {
        desc->getSet(frameIndex),
        //desc->getSet(frameIndex,1)
    };
    updateSetUniformBuffer(ctx, set[0], matrixBuffer);
    //updateSetTexture(ctx,set[1],texel);

    vkCmdBindDescriptorSets(cmd,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline.pipelineLayout,
        0,
        desc->setLayout.size(),
        set,
        0,
        nullptr
    );
    vkCmdDraw(cmd,vertices.size(),1,0,0);
}