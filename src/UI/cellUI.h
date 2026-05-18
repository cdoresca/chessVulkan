#pragma once
#include "VulkanWrapper/model.h"

class cellUI: public model{



    public:
        cellUI(renderer* r);
        void render(VkCommandBuffer cmd) const override;
};