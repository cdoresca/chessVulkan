#pragma once
#include"cell.h"


const int HEIGHT = 8;
const int WIDTH = 8;

class board{

    cell* grid[HEIGHT][WIDTH];

    
    void  cloneGrid(cell* other[HEIGHT][WIDTH]);

    void build();
    void cleanup();

    public :
        board();
        board(cell*[HEIGHT][WIDTH]);
        ~board();

        board* clone();

        board* clone(Move mv, pieceBase* piece);

        const cell* getCell(Position p);

};