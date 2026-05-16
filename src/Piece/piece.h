#pragma once

#include<string>
#include <vector>

#include "gameState.h"

enum Color { WHITE, BLACK };

class pieceBase{

    protected:
        std::string name;
        Position position;
        
        bool alive;
        Color color;

    public:

        pieceBase(const char* name, Position pos,Color color,bool alive = true);

        virtual std::vector<Position> generateMoves(const gameState* state) = 0;

        Color getColor() const;
        const char* getName();
        Position getPosition();

        virtual pieceBase* clone() const;
        virtual pieceBase* clone(Move mv) const;


};