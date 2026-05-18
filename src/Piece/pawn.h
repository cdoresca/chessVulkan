#pragma once

#include<vector>

#include "piece.h"
#include"cell.h"

class pawn:public  pieceBase{

    int dir;
    std::vector<Position> kill();
    std::vector<Position> straight();

    public:
        pawn(const char* name, Position pos,Color color,bool alive = true);
    
        std::vector<Position> generateMoves(const gameState* state) override;

        pieceBase* clone() const override;
        pieceBase* clone(Move mv) const override;
};