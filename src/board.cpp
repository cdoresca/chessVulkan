#include "board.h"
#include "Piece/pawn.h"
#include "Piece/rook.h"
#include "Piece/king.h"
#include "Piece/bishop.h"
#include "Piece/knight.h"
#include "Piece/queen.h"

board::board(){

}

board::board(cell* other[HEIGHT][WIDTH])
{
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            grid[i][j] = other[i][j]->clone();
        }
    }
}

board::~board(){ cleanup(); }

board* board::clone()
{
    return new board(grid);
}

board* board::clone(Move mv, pieceBase* piece){
    cell* newBoard[HEIGHT][WIDTH];
    cloneGrid(newBoard);

    cell* oldfrom = newBoard[mv.from.row][mv.from.column];
    cell* oldto = newBoard[mv.to.row][mv.to.column];
    newBoard[mv.from.row][mv.from.column] = newBoard[mv.from.row][mv.from.column]->clone(nullptr);
    newBoard[mv.to.row][mv.to.column] = newBoard[mv.to.row][mv.to.column]->clone(piece);

    delete oldfrom;
    delete oldto;

    return new board(newBoard);
}

const cell* board::getCell(Position p){
    return grid[p.row][p.column];
}

void board::build(){
    
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
            grid[i][j] = new cell({i,j},nullptr);
        
    for (int i = 0; i < 8; i++)
            grid[6][i]->setPiece(new pawn("PionBlanc" + i, {6, i}, WHITE, true));

        // Tours blanches
        grid[7][0]->setPiece(new rook("TourBlanc0", {7, 0}, WHITE, true));
        grid[7][7]->setPiece(new rook("TourBlanc1", {7, 7}, WHITE, true));

        // Cavaliers blancs
        grid[7][2]->setPiece( new knight("ChevalBlanc0", {7, 2}, WHITE, true));
        grid[7][5]->setPiece( new knight("ChevalBlanc1", {7, 5}, WHITE, true));

        // Fous blancs
        grid[7][1]->setPiece(new bishop("FouBlanc0", {7, 1}, WHITE, true));
        grid[7][6]->setPiece(new bishop("FouBlanc1", {7, 6}, WHITE, true));

        // Roi blanc
        grid[7][3]->setPiece(new king("RoiBlanc", {7, 3}, WHITE, true));

        // Reine blanche
        grid[7][4]->setPiece(new queen("ReineBlanc", {7, 4}, WHITE, true));
    
    for (int i = 0; i < 8; i++)
            grid[1][i]->setPiece(new pawn("PionNoir" + i, {6, i}, BLACK, true));

        // Tours Noir
        grid[0][0]->setPiece(new rook("TourNoir0", {0, 0}, BLACK, true));
        grid[0][7]->setPiece(new rook("TourNoir1", {0, 7}, BLACK, true));

        // Cavaliers Noir
        grid[0][2]->setPiece( new knight("ChevalNoir0", {0, 2}, BLACK, true));
        grid[0][5]->setPiece( new knight("ChevalNoir1", {0, 5}, BLACK, true));

        // Fous Noir
        grid[0][1]->setPiece(new bishop("FouNoir0", {0, 1}, BLACK, true));
        grid[0][6]->setPiece(new bishop("FouNoir1", {0, 6}, BLACK, true));

        // Roi Noir
        grid[0][3]->setPiece(new king("RoiNoir", {0, 3}, BLACK, true));

        // Reine Noir
        grid[0][4]->setPiece(new queen("ReineNoir", {0, 4}, BLACK, true));
    
}

void board::cleanup(){
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
            delete grid[i][j];
}

void board::cloneGrid(cell* other[HEIGHT][WIDTH]){

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            other[i][j] = grid[i][j]->clone();
        }
    }
        
}