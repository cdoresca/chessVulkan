#pragma once 
#include"vertex.h"
#include "renderer.h"



class model{

    protected: 
        renderer* render;
        std::vector<Vertex> vertices;
        Buffer verticesBuffer;
        Texture texel;

    public:
        virtual void render();


};

std::vector<Vertex> quad(uint32_t height,uint32_t width);