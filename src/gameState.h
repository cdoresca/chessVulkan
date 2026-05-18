#pragma once

#include<vector>

#include "board.h"
#include "playerState.h"



class gameState{

    board* m_board;
    playerState* white;
    playerState* black;
    Color turn;

    public:
        gameState(board*,playerState* white,playerState* black,Color);
        

        gameState* ApplyMove(Move mv) const;

        bool check(Color color) const;

        std::vector<Move> generateLegalMove();

        const cell* getBoard(Position p) const;
        void setBoard(board*);
        void setTurn(Color color);

        gameState* clone() const;

        playerState* getPlayer(Color color) const;

};