#include "cellUI.h"

cellUI::cellUI(renderer* render):model(render){

    texel = render_->uploadTexture(IMAGE_DIR"/square brown dark_1x_ns.png");
}

void cellUI::render(VkCommandBuffer cmd, uint32_t frameIndex,descriptor* desc) const
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &verticesBuffer.buffer,&offset);
    VkDescriptorSet set[] = {
        desc->getSet(frameIndex),
        desc->getSet(frameIndex,1)
    };
    render_->updateSetUBuffer(set[0], matrixBuffer);
    render_->updateTexture(set[1],texel);

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