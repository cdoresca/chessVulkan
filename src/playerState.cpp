#include"playerState.h"

playerState::playerState(std::vector<pieceBase*> p)
{ 
    for(int i = 0; i <p.size(); i++)
        pieces[p[i]->getName()] = p[i];
}

void playerState::update(pieceBase* p){ 
    pieceBase* old = pieces[p->getName()];
    pieces[p->getName()] = p;
    delete old;
}

void playerState::remove(pieceBase* p){
    for(auto& [key, value]:pieces){

        if(value->getPosition() == p->getPosition()) {
            pieces.erase(key);
            delete value;
        }
    }
}