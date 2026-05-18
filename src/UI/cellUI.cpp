#include "cellUI.h"

cellUI::cellUI(renderer* render):model(render){

    texel = render_->uploadTexture(IMAGE_DIR"/square brown dark_1x_ns.png");
}

void cellUI::render(VkCommandBuffer cmd) const
{
    vkCmdBindVertexBuffers(cmd, 0, 1, &verticesBuffer.buffer,0);
    vkCmdDraw(cmd,vertices.size(),1,0,0);
}