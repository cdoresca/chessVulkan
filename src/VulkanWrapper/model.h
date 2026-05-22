#pragma once 
#include"vertex.h"
#include "buffer.h"
#include "pipeline.h"




class model{

    void cleanup();

    protected: 
        VkContext ctx;
        VkPipelineDescription pipeline;
        VkCommand* cmd;

        std::vector<Vertex> vertices;
        Buffer verticesBuffer;

        Texture texel;

        glm::mat4 matrix;
        Buffer matrixBuffer;

    public:
        model(VkContext ctx, VkPipelineDescription pipeline, VkCommand* command);
        ~model();
        virtual void render(VkCommandBuffer cmd, uint32_t frameIndex, descriptor* desc)const = 0;

};

std::vector<Vertex> quad(uint32_t height = 5,uint32_t width = 5);