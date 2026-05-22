#pragma once
#include<vector>
#include "../move.h"

class gameState;

enum Color { WHITE, BLACK };


class pieceBase{

    protected:
        const char* name;
        Position position;
        
        bool alive;
        Color color;

    public:

        pieceBase(const char* name, Position pos,Color color,bool alive = true);

        virtual std::vector<Position> generateMoves(const gameState* state) = 0;

        Color getColor() const;
        const char* getName();
        Position getPosition();

        virtual pieceBase* clone() const = 0;
        virtual pieceBase* clone(Move mv) const = 0;


};