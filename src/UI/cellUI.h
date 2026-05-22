#pragma once
#include "VulkanWrapper/model.h"

class cellUI: public model{



    public:
        cellUI(VkContext ctx, VkPipelineDescription pipeline, VkCommand* command);
        void render(VkCommandBuffer cmd, uint32_t frameIndex , descriptor* desc) const override;
};