#include"bishop.h"
#include "gameState.h"
bishop::bishop(const char* name, Position pos,Color color,bool alive):pieceBase(name, pos, color, alive){ }

std::vector<Position> bishop::generateMoves(const gameState* state){

    std::vector<Position> moves;
    int row = position.row;
    int col = position.column;

    int index[8][2] = { { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } };
    int nx;
    int ny;

    for (int j = 0; j < 4; j++)
    {
        nx = index[j][0] + row;
        ny = index[j][1] + col;

        while ((0 <= nx && nx < 8) && (0 <= ny && ny < 8))
        {

            if (!state->getBoard({nx,ny})->empty())
            {
                if (state->getBoard({nx,ny})->getPiece()->getColor() != color) moves.push_back({nx,ny}); 
                break;
            }
            moves.push_back({nx,ny}); 
            nx += index[j][0];
            ny += index[j][1];

        }
    }
    return moves;

}

pieceBase* bishop::clone() const{
    return new bishop(name,position,color,alive);
}
pieceBase* bishop::clone(Move mv) const {
    return new bishop(name, mv.to,color,alive);
}