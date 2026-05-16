#pragma once
#include<list>

class IObserver{

    virtual void onNotify() = 0;
};

class Subject{

    std::list<IObserver*> obsevers;

    void addObserver(IObserver* o){ obsevers.push_back(o); }
    void removeObserver(IObserver* o){ obsevers.remove(o); }
};