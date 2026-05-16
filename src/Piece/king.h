#include "piece.h"

class king: public pieceBase{

    std::vector<Position> mouvement();
    
    public:
        king(const char* name, Position pos,Color color,bool alive = true);
        std::vector<Position> generateMoves(const gameState* state) override;

};