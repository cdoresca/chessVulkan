#include"pawn.h"

pawn::pawn(const char* name, Position pos,Color color,bool alive = true):pieceBase(name, pos, color, alive){ }

std::vector<Position> pawn::straight(){
    std::vector<Position> moves;

    int row = position.row;
    int col = position.column;

    int step = row + dir;
    if(step>= 0 && step<8) moves.push_back({step,col});

    int start = color == WHITE ? 6 : 1;
    if (start != row) return moves;

    step += dir;
    moves.push_back({step,col});

    return moves;
}

std::vector<Position> pawn::kill(){
    std::vector<Position> moves;

    int row = position.row;
    int col = position.column;

    int step = row + dir;

    if (step < 0 || step > 7) return  moves;

    if (col > 0)  moves.push_back({step, col - 1});
    
    if (col < 7) moves.push_back({step, col + 1});

    return moves;
    
}

std::vector<Position> pawn::generateMoves(const gameState* state){
    std::vector<Position> moves;

    for(Position pos : straight()){
        if(state->getBoard(pos)->empty()) moves.push_back(pos);
        if(!state->getBoard(pos)->empty()) break;
    }

    for(Position pos : kill()){
        if(!state->getBoard(pos)->empty() &&
        color != state->getBoard(pos)->getPiece()->getColor()) 
            moves.push_back(pos);
    }
    
}