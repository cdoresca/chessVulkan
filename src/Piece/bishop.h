#include "piece.h"

class bishop:public pieceBase{

    public:
        bishop(const char* name, Position pos,Color color,bool alive = true);
        std::vector<Position> generateMoves(const gameState* state) override;
};