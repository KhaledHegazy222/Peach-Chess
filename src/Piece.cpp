


#include "Piece.hpp"
#include "Board.hpp"



Piece::Piece(){}


void Piece::init(PieceType t,Color cl,int r,int c){
    type = t;
    color = cl;
    row = r;
    col = c;
}

Piece Piece::operator=(const Piece& other){
    type = other.type;
    row = other.row;
    col = other.col;
    color = other.color;
    return *this;
}