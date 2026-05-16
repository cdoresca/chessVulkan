#include"piece.h"

pieceBase::pieceBase(const char* name, Position pos,Color color,bool alive):name(name),position(pos),color(color),alive(alive)
{ }

Color pieceBase::getColor() const {return color;}