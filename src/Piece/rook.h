#include "piece.h"

class rook:public pieceBase{

    public:
        rook(const char* name, Position pos,Color color,bool alive = true);
        std::vector<Position> generateMoves(const gameState* state) override;
    
};