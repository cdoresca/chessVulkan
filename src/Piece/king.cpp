#include "king.h"

king::king(const char* name, Position pos,Color color,bool alive = true):pieceBase(name, pos, color, alive){ }

std::vector<Position> king::generateMoves(const gameState* state)
{
    std::vector<Position> moves;

    for(Position pos : mouvement())
    {
        if(state->getBoard(pos)->empty()) moves.push_back(pos);
        if(!state->getBoard(pos)->empty())
            if (state->getBoard(pos)->getPiece()->getColor() != color)
                moves.push_back(pos);
    }
    return moves;
}

std::vector<Position> king::mouvement(){
    std::vector<Position> moves;

    int row = position.row;
    int col = position.column;

    int index[3] = { -1, 0, 1 };

    for(int i : index)
    {
        for(int j : index)
        {
            if (i != 0 || j != 0)
            {
                int nx = i + row;
                int ny = j + col;

                if ((0 <= nx && nx < 8) && (0 <= ny && ny < 8))
                    moves.push_back({nx,ny});
            }
        }
    }

    return moves;
}
