#pragma once 

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
template<typename T>
class IBuilder{
    
    public:

        IBuilder();
        virtual void build() = 0;
        virtual void reset() = 0;
        virtual T get() = 0;
};

template<typename T>
class director
{
    IBuilder* builder;

    public:
        director(IBuilder* builder);
        void make();
        
};