#include"piece.h"
#include "gameState.h"
pieceBase::pieceBase(const char* name, Position pos,Color color,bool alive):name(name),position(pos),color(color),alive(alive)
{ }

Color pieceBase::getColor() const {return color;}

const char* pieceBase::getName(){ return name; }
Position pieceBase::getPosition(){ return position; }