#include "cell.h"
#include "Piece/piece.h"

cell::cell(Position pos, pieceBase* p ): pos(pos),piece(p){ }

cell::~cell(){ cleanup(); }

bool cell::empty() const { return piece == nullptr; }

cell* cell::clone(pieceBase* p) const{ return new cell(pos, p); }

cell* cell::clone() const{ return new cell(pos, piece); }

const pieceBase* cell::getPiece() const{ return piece;} 

void cell::cleanup(){ delete piece;}

void cell::setPiece(pieceBase* p){ piece = p; }


bool Position::operator==(const Position& other) const{
    return row == other.row && column == other.column;
}