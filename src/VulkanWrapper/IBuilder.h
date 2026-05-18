#pragma once 


template<typename T>
class IBuilder{
    
    public:

        IBuilder() = default;
        ~IBuilder() = default;
        virtual void build() = 0;
        virtual void reset() = 0;
        virtual T get() = 0;
};

template<typename T>
class director
{
    IBuilder<T>* builder;

    public:
        director(IBuilder<T>* builder):builder(builder){}
        void make(){builder->build();}
        
};