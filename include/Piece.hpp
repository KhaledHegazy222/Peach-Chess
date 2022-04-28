#pragma once



enum PieceType {Empty,King,Queen,Bishop,Pawn,Knight,Rook};
enum Color {Black,White};




class Piece{


    private:

    public:
        int row;
        int col;
        PieceType type;
        Color color;

        Piece();
        void init(PieceType t,Color cl,int r,int c);
        

        Piece operator=(const Piece& other);

        


};


