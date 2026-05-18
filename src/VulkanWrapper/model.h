#pragma once 
#include"vertex.h"
#include "renderer.h"



class model{

    protected: 
        renderer* render_;
        std::vector<Vertex> vertices;
        Buffer verticesBuffer;
        Texture texel;

    public:
        model(renderer* render);
        virtual void render(VkCommandBuffer cmd)const = 0;

};

std::vector<Vertex> quad(uint32_t height = 5,uint32_t width = 5);