#pragma once


struct Position{

    int row;
    int column;
    
    bool operator==(const Position& other) const;
};


struct Move{

    Position from;
    Position to;
   
};