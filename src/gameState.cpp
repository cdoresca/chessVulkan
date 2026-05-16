#include"gameState.h"

gameState::gameState(board* b,playerState* white,playerState* black,Color c): m_board(b),white(white),black(black),turn(c)
{
    
}

const cell* gameState::getBoard(Position p) const
{
    return m_board->getCell(p);
}

gameState* gameState::ApplyMove(Move mv) const
{
    Position from = mv.from;
    Position to = mv.to;

    gameState* newState = clone();

    pieceBase* mvPiece = newState->getBoard(from)->getPiece()->clone(mv);
    newState->setBoard(m_board->clone(mv,mvPiece));
    Color newTurn = turn == WHITE ? BLACK : WHITE;
    newState->setTurn(newTurn);
    newState->getPlayer(newTurn)->update(mvPiece);
    newState->getPlayer(turn)->remove(mvPiece);
 
    return newState;
}

void gameState::setTurn(Color color){ turn = color; }

void gameState::setBoard(board* b){ m_board = b;}

playerState* gameState::getPlayer(Color color) const {return turn == WHITE ? black : white;}

std::vector<Move> gameState::generateLegalMove(){
    std::vector<Move> moves;
    playerState* player = getPlayer(turn);
    std::unordered_map<const char*, pieceBase*> pieces =  player->getPieces();
    for(const auto& piece : pieces)
    {
        std::vector<Position> positions = piece.second->generateMoves(this);
        for(int i = 0; i < positions.size(); i++){
            Move move = { .from = piece.second->getPosition(), .to = positions[i] };

            gameState* newState = ApplyMove(move);
            if(!newState->check(turn)){
                moves.push_back(move);
            }

            delete newState;
        }
    }

    return moves;
}

gameState* gameState::clone() const { return new gameState(m_board->clone(),white,black, turn);}

bool gameState::check(Color color) const
{
    playerState* defense = color == WHITE ? white : black;
    playerState* attack = color == WHITE ? black : white;

    std::unordered_map<const char*, pieceBase*> defensePieces =  defense->getPieces();
    std::unordered_map<const char*, pieceBase*> attackPieces =  attack->getPieces();
    for(const auto& piece : attackPieces)
    {
        std::vector<Position> positions = piece.second->generateMoves(this);
        for(int i = 0; i < positions.size(); i++){
            if(positions[i] == defensePieces["roi"]->getPosition()) return true;
        }
    }

    return false;
}