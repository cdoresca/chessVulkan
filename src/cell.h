#pragma once
#include "Piece/piece.h"

struct Position{

    int row;
    int column;
    
    bool operator==(const Position& other) const;
};

class cell{
    Position pos;

    pieceBase* piece;

    void cleanup();

    public:
        
        cell(Position pos, pieceBase* p = nullptr);
        ~cell(); 

        bool empty() const;

        cell* clone() const;
        cell* clone(pieceBase* piece) const ;

       
        const pieceBase* getPiece() const;
        void setPiece(pieceBase*);
};