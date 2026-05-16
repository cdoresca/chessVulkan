#include "knight.h"

knight::knight(const char* name, Position pos,Color color,bool alive = true):pieceBase(name, pos, color, alive){ }

std::vector<Position> knight::mouvement(){
    std::vector<Position> moves;

    int row = position.row;
    int col = position.column;

    int index[8][2] = { 
        { 2, 1 }, 
        { 2, -1 }, 
        { -2, 1 }, 
        { -2, -1}, 
        { 1, 2 }, 
        { 1, -2 }, 
        { -1, 2 }, 
        { -1, -2 } 
    };

    int nx;
    int ny;

    for (int j = 0; j < 8; j++)
    {
        nx = index[j][0] + row;
        ny = index[j][1] + col;

        if ((0 <= nx && nx < 8) && (0 <= ny && ny < 8)) moves.push_back({nx,ny});
    }
}

std::vector<Position> knight::generateMoves(const gameState* state)
{
    std::vector<Position> moves;

    for(Position pos : mouvement()){
        if (state->getBoard(pos)->empty() ||
            (!state->getBoard(pos)->empty()
            && color != state->getBoard(pos)->getPiece()->getColor()))
            moves.push_back(pos);
    }

}