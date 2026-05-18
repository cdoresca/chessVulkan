#include"queen.h"
#include "gameState.h"
queen::queen(const char* name, Position pos,Color color,bool alive):pieceBase(name, pos, color, alive){ }

std::vector<Position> queen::generateMoves(const gameState* state){

    std::vector<Position> moves;
    int row = position.row;
    int col = position.column;

    int index[8][2] = { { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
    int nx;
    int ny;

    for (int j = 0; j < 8; j++)
    {
        nx = index[j][0] + row;
        ny = index[j][1] + col;

        while ((0 <= nx && nx < 8) && (0 <= ny && ny < 8))
        {

            if (!state->getBoard({nx, ny})->empty())
            {
                if(state->getBoard({nx, ny})->getPiece()->getColor() != color)moves.push_back({nx,ny});

                break;
            }
            moves.push_back({nx,ny});
            nx += index[j][0];
            ny += index[j][1];

        }
    }
    return moves;
}

pieceBase* queen::clone() const{
    return new queen(name,position,color,alive);
}
pieceBase* queen::clone(Move mv) const {
    return new queen(name, mv.to,color,alive);
}