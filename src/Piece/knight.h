#include "piece.h"

class knight:public pieceBase{
    
    std::vector<Position> mouvement();

    public:
        knight(const char* name, Position pos,Color color,bool alive = true);
        std::vector<Position> generateMoves(const gameState* state) override;

        pieceBase* clone() const override;
        pieceBase* clone(Move mv) const override;
};