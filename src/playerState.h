#pragma once 
#include<unordered_map>
#include "Piece/piece.h"

class playerState{

    std::unordered_map<const char*, pieceBase*> pieces;

    public:
        playerState(std::vector<pieceBase*>);

        void update(pieceBase*);
        void remove(pieceBase*);

      std::unordered_map<const char*, pieceBase*> getPieces();  
};