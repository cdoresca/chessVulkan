#include "piece.h"

class queen:public pieceBase{
    
    public:
        queen(const char* name, Position pos,Color color,bool alive = true);
        std::vector<Position> generateMoves(const gameState* state) override;

        pieceBase* clone() const override;
        pieceBase* clone(Move mv) const override;

};