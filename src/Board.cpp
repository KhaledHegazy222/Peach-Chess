
#include "Board.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <utility>
#include <algorithm>


int TileDimensions = 60;
int PaddingLeft,PaddingTop;



SDL_Texture* movingPieceTexture = NULL;
Piece movingPiece;
std::set<std::pair<int,int>>availableMoves;
bool MouseDown = false;
std::pair<int,int>lastMoveFrom(-1,-1);
std::pair<int,int>lastMoveTo(-1,-1);

bool WhiteChecked = false;
bool BlackChecked = false;


SDL_Texture* BTile;
SDL_Texture* WTile;
SDL_Texture* MoveTile;
SDL_Texture* MoveCircle;
SDL_Texture* TakeTile;
SDL_Texture* WLastMove;
SDL_Texture* BLastMove;
SDL_Texture* CheckTile;


SDL_Texture* BKing;
SDL_Texture* BQueen;
SDL_Texture* BKnight;
SDL_Texture* BBishop;
SDL_Texture* BRook;
SDL_Texture* BPawn;

SDL_Texture* WKing;
SDL_Texture* WQueen;
SDL_Texture* WKnight;
SDL_Texture* WBishop;
SDL_Texture* WRook;
SDL_Texture* WPawn;



Board::Board(){}

Board::Board(int width,int height){
    BKing = TextureManager::loadTexture("../assets/BKing.png");
    BQueen = TextureManager::loadTexture("../assets/BQueen.png");
    BKnight = TextureManager::loadTexture("../assets/BKnight.png");
    BBishop = TextureManager::loadTexture("../assets/BBishop.png");
    BRook = TextureManager::loadTexture("../assets/BRook.png");
    BPawn = TextureManager::loadTexture("../assets/BPawn.png");

    WKing = TextureManager::loadTexture("../assets/WKing.png");
    WQueen = TextureManager::loadTexture("../assets/WQueen.png");
    WKnight = TextureManager::loadTexture("../assets/WKnight.png");
    WBishop = TextureManager::loadTexture("../assets/WBishop.png");
    WRook = TextureManager::loadTexture("../assets/WRook.png");
    WPawn = TextureManager::loadTexture("../assets/WPawn.png");

    BTile = TextureManager::loadTexture("../assets/BTile.png");
    WTile = TextureManager::loadTexture("../assets/WTile.png");
    MoveTile = TextureManager::loadTexture("../assets/MoveTile.png");
    MoveCircle = TextureManager::loadTexture("../assets/MoveCircle.png");
    TakeTile = TextureManager::loadTexture("../assets/TakeTile.png");
    WLastMove = TextureManager::loadTexture("../assets/WLastMove.png");
    BLastMove = TextureManager::loadTexture("../assets/BLastMove.png");
    CheckTile = TextureManager::loadTexture("../assets/CheckTile.png");

    
    TileDimensions = std::min(height / 8,width / 8);
    PaddingLeft = (Game::width - (TileDimensions * 8)) / 2;
    PaddingTop = (Game::height - (TileDimensions * 8)) / 2;
    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++)
            board[i][j].init(Empty,Black,i,j);
    }



}

void Board::init(const char* fen){
    int row = 0,col = 0;
    int i;
    for(int row = 0;row < 8;row++)
        for(int col = 0;col < 8;col++)
            board[row][col].type = Empty;
    
    for(i = 0;row != 8;i++){
        if(fen[i] == '/')
            continue;

        if(SDL_isdigit(fen[i])){
            col += fen[i] - '0';
        }
        else{
            switch (fen[i]){
                case 'K':
                    board[row][col].init(King,White,row,col);    
                    break;
                case 'k':
                    board[row][col].init(King,Black,row,col);    
                    break;
                case 'Q':
                    board[row][col].init(Queen,White,row,col);    
                    break;
                case 'q':
                    board[row][col].init(Queen,Black,row,col);    
                    break;
                case 'N':
                    board[row][col].init(Knight,White,row,col);    
                    break;
                case 'n':
                    board[row][col].init(Knight,Black,row,col);    
                    break;
                case 'B':
                    board[row][col].init(Bishop,White,row,col);    
                    break;
                case 'b':
                    board[row][col].init(Bishop,Black,row,col);    
                    break;
                case 'R':
                    board[row][col].init(Rook,White,row,col);    
                    break;
                case 'r':
                    board[row][col].init(Rook,Black,row,col);    
                    break;
                case 'P':
                    board[row][col].init(Pawn,White,row,col);    
                    break;
                case 'p':
                    board[row][col].init(Pawn,Black,row,col);    
                    break;
            
                default:
                    break;
            }
            col++;

        }
        row += col / 8;
        col %= 8;
    }
    i++;
    if(fen[i]=='w')
        turn = White;
    else if(fen[i] == 'b')  
        turn = Black;


    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++){
            if(board[i][j].type == King){
                if(board[i][j].color == White)
                    WhiteKing = board[i][j];
                else
                    BlackKing = board[i][j];
            }
        }
    }
    

}

void Board::render(){ 
    // Render Tiles (Black / White)
    for(int row = 0;row < 8;row++){
        for(int col = 0;col < 8;col++){
            SDL_Rect src,dest;
            src.h = src.w = 60;
            src.x = src.y = 0;
            
            dest.h = dest.w = TileDimensions;
            dest.x = PaddingLeft + TileDimensions * col;
            dest.y = PaddingTop + TileDimensions * row;
            if((row + col) % 2 == 0){ // White Tile
                TextureManager::drawTexture(WTile,src,dest);
            }
            else{
                TextureManager::drawTexture(BTile,src,dest);
            }

        }
    }
    
    if(lastMoveFrom.first >= 0){
        SDL_Rect src,dest;
        src.h = src.w = 60;
        src.x = src.y = 0;
        
        dest.h = dest.w = TileDimensions;
        dest.y = PaddingTop + TileDimensions * lastMoveFrom.first;
        dest.x = PaddingLeft + TileDimensions * lastMoveFrom.second;
        TextureManager::drawTexture(((lastMoveFrom.first + lastMoveFrom.second) % 2 == 0 ? WLastMove : BLastMove),src,dest);
        
        
        dest.y = PaddingTop + TileDimensions * lastMoveTo.first;
        dest.x = PaddingLeft + TileDimensions * lastMoveTo.second;
        TextureManager::drawTexture(((lastMoveTo.first + lastMoveTo.second )% 2 == 0 ? WLastMove : BLastMove),src,dest);
        
    }

    if(movingPiece.type != Empty){
        SDL_Rect src,dest;
        src.h = src.w = 60;
        src.x = src.y = 0;
        
        dest.h = dest.w = TileDimensions;
        dest.y = PaddingTop + TileDimensions * movingPiece.row;
        dest.x = PaddingLeft + TileDimensions * movingPiece.col;
        TextureManager::drawTexture(MoveTile,src,dest);
        
        for(auto move : availableMoves){
            if(board[move.first][move.second].type != Empty){
                src.h = src.w = 60;
                src.x = src.y = 0;    
                dest.h = dest.w = TileDimensions;
                dest.y = PaddingTop + TileDimensions * move.first;
                dest.x = PaddingLeft + TileDimensions * move.second;
                TextureManager::drawTexture(TakeTile,src,dest);
            }
            else{
                src.h = src.w = 1200;
                src.x = src.y = 0;    
                dest.h = dest.w = TileDimensions / 3;
                dest.y = PaddingTop + TileDimensions * move.first + (1 - (1.0 / 3)) / 2 * TileDimensions;
                dest.x = PaddingLeft + TileDimensions * move.second + (1 - (1.0 / 3)) / 2 * TileDimensions;
                TextureManager::drawTexture(MoveCircle,src,dest);
            }
        }
    }
   
    if(WhiteChecked){
        SDL_Rect src,dest;
        src.x = src.y = 0;
        src.h = src.w = 60;
        dest.h = dest.w = TileDimensions;
        dest.x = PaddingLeft + TileDimensions * WhiteKing.col;
        dest.y = PaddingTop + TileDimensions * WhiteKing.row;   
        TextureManager::drawTexture(CheckTile,src,dest); 
    }
    if(BlackChecked){
        SDL_Rect src,dest;
        src.x = src.y = 0;
        src.h = src.w = 60;
        dest.h = dest.w = TileDimensions;
        dest.x = PaddingLeft + TileDimensions * BlackKing.col;
        dest.y = PaddingTop + TileDimensions * BlackKing.row;   
        TextureManager::drawTexture(CheckTile,src,dest); 
    }
    
    // Render Pieces 
    for(int row = 0;row < 8;row++){
        for(int col = 0;col < 8;col++){
            SDL_Rect src,dest;
            src.h = src.w = 60;
            src.x = src.y = 0;
            
            dest.h = dest.w = TileDimensions;
            dest.x = PaddingLeft + TileDimensions * col;
            dest.y = PaddingTop + TileDimensions * row;
            if(board[row][col].color == Black){
                switch (board[row][col].type){
                    case King:
                        TextureManager::drawTexture(BKing,src,dest);
                        break;
                    case Queen:
                        TextureManager::drawTexture(BQueen,src,dest);
                        break;
                    case Knight:
                        TextureManager::drawTexture(BKnight,src,dest);
                        break;
                    case Bishop:
                        TextureManager::drawTexture(BBishop,src,dest);
                        break;
                    case Rook:
                        TextureManager::drawTexture(BRook,src,dest);
                        break;
                    case Pawn:
                        TextureManager::drawTexture(BPawn,src,dest);
                        break;

                    default:
                        break;
                }
            }
            else if(board[row][col].color == White){
                switch (board[row][col].type){
                    case King:
                        TextureManager::drawTexture(WKing,src,dest);
                        break;
                    case Queen:
                        TextureManager::drawTexture(WQueen,src,dest);
                        break;
                    case Knight:
                        TextureManager::drawTexture(WKnight,src,dest);
                        break;
                    case Bishop:
                        TextureManager::drawTexture(WBishop,src,dest);
                        break;
                    case Rook:
                        TextureManager::drawTexture(WRook,src,dest);
                        break;
                    case Pawn:
                        TextureManager::drawTexture(WPawn,src,dest);
                        break;

                    default:
                        break;
                }
            }


        }
    }
    
    // Render Moving Piece
    if(movingPiece.type != Empty){
        SDL_Rect dest;
        dest.h = dest.w = TileDimensions;
        SDL_GetMouseState(&(dest.x),&(dest.y));
        dest.x -= TileDimensions / 2;
        dest.y -= TileDimensions / 2;
        SDL_RenderCopy(Game::renderer,movingPieceTexture,NULL,&dest);
    }
}

void Board::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    int mouseX,mouseY;
    switch (event.type){
        case SDL_MOUSEBUTTONDOWN:
            if(!MouseDown){
                SDL_GetMouseState(&mouseX,&mouseY);
                move((mouseY - PaddingTop) / TileDimensions,(mouseX - PaddingLeft) / TileDimensions);
                MouseDown = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(MouseDown){
                SDL_GetMouseState(&mouseX,&mouseY);
                release((mouseY - PaddingTop) / TileDimensions,(mouseX - PaddingLeft) / TileDimensions);
                MouseDown = false;
            }
            
        default:
            break;
    }
}

void Board::update(){
    if( !MouseDown &&  (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1)))
        release(-1,-1);
    if(MouseDown && !(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1)))
        release(-1,-1);
    
}

void Board::move(int row,int col){
    movingPiece.type = Empty;
    if(!validPos(row,col) || board[row][col].color != turn)   
        return;
    if(board[row][col].type == Empty)
        return;


    
    if(board[row][col].color == Black){
        switch (board[row][col].type){
            case King:
                movingPieceTexture = BKing;
                break;
            case Queen:
                movingPieceTexture = BQueen;
                break;
            case Knight:
                movingPieceTexture = BKnight;
                break;
            case Bishop:
                movingPieceTexture = BBishop;
                break;
            case Rook:
                movingPieceTexture = BRook;
                break;
            case Pawn:
                movingPieceTexture = BPawn;
                break;

            default:
                break;
        }
    }
    else if(board[row][col].color == White){
        switch (board[row][col].type){
            case King:
                movingPieceTexture = WKing;
                break;
            case Queen:
                movingPieceTexture = WQueen;
                break;
            case Knight:
                movingPieceTexture = WKnight;
                break;
            case Bishop:
                movingPieceTexture = WBishop;
                break;
            case Rook:
                movingPieceTexture = WRook;
                break;
            case Pawn:
                movingPieceTexture = WPawn;
                break;

            default:
                break;
        }
    }
    
    
    movingPiece = board[row][col];
    availableMoves = sudoLegalMoves(movingPiece,1);
    std::set<std::pair<int,int>>temp;
    for(auto elem : availableMoves){
        Piece from,to;
        from = movingPiece;
        to = board[elem.first][elem.second];

        performMove(movingPiece,elem.first,elem.second,0);
        
        if(turn == White && !isChecked(White))
            temp.insert(elem);
            
        if(turn == Black && !isChecked(Black))
            temp.insert(elem);

        
        
        reverseMove(from,to);
    }
    
    availableMoves = temp;
    board[row][col].type = Empty;
}

void Board::release(int row,int col){
    if(movingPiece.type == Empty)
        return;
    if((row == movingPiece.row && col == movingPiece.col) || !validPos(row,col) || availableMoves.find({row,col}) == availableMoves.end() ) {
        board[movingPiece.row][movingPiece.col] = movingPiece;
        movingPiece.type = Empty;
        availableMoves.clear();
        return;
    }
    
    
    if(movingPiece.type == King){
        if(movingPiece.color == White){
            WhiteCastleKingSide = WhiteCastleQueenSide = false;
        }
        else{
            BlackCastleKingSide = BlackCastleQueenSide = false;
        }
    }    




    if(board[0][0].type != Rook || board[0][0].color != Black)
        BlackCastleQueenSide = false;
    if(board[0][7].type != Rook || board[0][7].color != Black)
        BlackCastleKingSide = false;
    if(board[7][0].type != Rook || board[7][0].color != White)
        WhiteCastleQueenSide = false;
    if(board[7][7].type != Rook || board[7][7].color != White)
        WhiteCastleQueenSide = false;




    lastMoveFrom = {movingPiece.row,movingPiece.col};
    lastMoveTo = {row,col};
    performMove(movingPiece,row,col,1);

    movingPiece.type = Empty;
    availableMoves.clear();

    WhiteChecked = isChecked(White);
    BlackChecked = isChecked(Black);


    turn = (turn == White ? Black : White);

    std::cout << "Last Move From ---> " << lastMoveFrom.first << " " << lastMoveFrom.second << std::endl;
    std::cout << "Last Move To ---> " << lastMoveTo.first << " " << lastMoveTo.second << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;


}

bool Board::validPos(int row,int col){
    return row < 8 && row >= 0 && col < 8 && col >= 0;
}

std::set<std::pair<int,int>>Board::sudoLegalMoves(Piece piece,bool movement){
    int row = piece.row;
    int col = piece.col;
    PieceType type = piece.type;
    Color color = piece.color;
    std::set<std::pair<int,int>>moves;

    if(type == King){
        int dx[] = {1,0,-1,0};
        int dy[] = {0,1,0,-1};
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                int newRow = row + dx[i];
                int newCol = col + dy[j];
                if((newRow == row && newCol == col) || !validPos(newRow,newCol) || (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color))
                    continue;
                moves.insert({newRow,newCol});
            }
        }
            if(movement){
                if(color == White){
                    if(WhiteCastleKingSide && board[WhiteKing.row][WhiteKing.col + 1].type == Empty && board[WhiteKing.row][WhiteKing.col + 2].type == Empty && board[WhiteKing.row][WhiteKing.col + 3].type == Rook ){
                        auto spots = attacked(White);
                        if(!spots.count({WhiteKing.row,WhiteKing.row}) && !spots.count({WhiteKing.row,WhiteKing.col + 1}) && !spots.count({WhiteKing.row,WhiteKing.col + 2}) ){
                            moves.insert({row,col + 2});
                        }
                    }
                    if(WhiteCastleQueenSide && board[WhiteKing.row][WhiteKing.col - 1].type == Empty && board[WhiteKing.row][WhiteKing.col - 2].type == Empty && board[WhiteKing.row][WhiteKing.col - 3].type == Empty && board[WhiteKing.row][WhiteKing.col - 4].type == Rook){
                        auto spots = attacked(White);
                        if(!spots.count({WhiteKing.row,WhiteKing.row}) && !spots.count({WhiteKing.row,WhiteKing.col - 1}) && !spots.count({WhiteKing.row,WhiteKing.col - 2}) ){
                            moves.insert({row,col - 2});
                        }
                    }
                }
                else if(color == Black){
                    if(BlackCastleKingSide && board[BlackKing.row][BlackKing.col + 1].type == Empty && board[BlackKing.row][BlackKing.col + 2].type == Empty && board[BlackKing.row][BlackKing.col + 3].type == Rook){
                        auto spots = attacked(Black);
                        if(!spots.count({BlackKing.row,BlackKing.row}) && !spots.count({BlackKing.row,BlackKing.col + 1}) && !spots.count({BlackKing.row,BlackKing.col + 2})){
                            moves.insert({row,col + 2});
                        }
                    }
                    if(BlackCastleQueenSide && board[BlackKing.row][BlackKing.col - 1].type == Empty && board[BlackKing.row][BlackKing.col - 2].type == Empty && board[BlackKing.row][BlackKing.col - 3].type == Empty && board[BlackKing.row][BlackKing.col - 4].type == Rook){
                        auto spots = attacked(Black);
                        if(!spots.count({BlackKing.row,BlackKing.row}) && !spots.count({BlackKing.row,BlackKing.col - 1}) && !spots.count({BlackKing.row,BlackKing.col - 2})){
                            moves.insert({row,col - 2});
                        }
                    }
                }
        }

    }
    else if(type == Knight){
        int d[] = {1,2,-1,-2};
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                if(std::abs(d[i]) == std::abs(d[j]))
                    continue;
                int newRow = row + d[i];
                int newCol = col + d[j];
                if(!validPos(newRow,newCol) || (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color) )
                    continue;
                moves.insert({newRow,newCol});
            }
        }
    }
    else if(type == Bishop){
        for(int i = 1;i <= 8;i++){
            int newRow = row + i;
            int newCol = col + i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row + i;
            int newCol = col - i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row - i;
            int newCol = col + i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row - i;
            int newCol = col - i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
    }
    else if(type == Rook){
        for(int i = 1;i <= 8;i++){
            int newRow = row + i;
            int newCol = col;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row - i;
            int newCol = col;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        
        for(int i = 1;i <= 8;i++){
            int newRow = row;
            int newCol = col + i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row;
            int newCol = col - i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
    }
    else if(type == Queen){
        for(int i = 1;i <= 8;i++){
            int newRow = row + i;
            int newCol = col + i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row + i;
            int newCol = col - i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row - i;
            int newCol = col + i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row - i;
            int newCol = col - i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row + i;
            int newCol = col;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row - i;
            int newCol = col;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        
        for(int i = 1;i <= 8;i++){
            int newRow = row;
            int newCol = col + i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
        for(int i = 1;i <= 8;i++){
            int newRow = row;
            int newCol = col - i;
            if( !validPos(newRow,newCol) ||  (board[newRow][newCol].type != Empty && board[newRow][newCol].color == piece.color ) )
                break;
            moves.insert({newRow,newCol});
            if(board[newRow][newCol].type != Empty)
                break;
        }
    }
    else if(type == Pawn){
        if(color == White){
            if(board[row - 1][col].type == Empty)
                moves.insert({row - 1,col});
            if(col != 7 && board[row - 1][col + 1].type != Empty && board[row - 1][col + 1].color != piece.color)
                moves.insert({row - 1,col + 1});
            if(col && board[row - 1][col - 1].type != Empty && board[row - 1][col - 1].color != piece.color)
                moves.insert({row - 1,col - 1});
            if(row == 6)
                moves.insert({row - 2,col});
            if(row == 3){
                if(col && board[row][col - 1].type == Pawn && board[row][col - 1].color == Black && lastMoveFrom.first == 1 && lastMoveFrom.second == col - 1){
                    moves.insert({row - 1,col - 1});
                }
                if(col + 1 < 7 && board[row][col + 1].type == Pawn && board[row][col + 1].color == Black && lastMoveFrom.first == 1  && lastMoveFrom.second == col + 1){
                    moves.insert({row - 1,col + 1});
                }
            }
            
        }
        else if(color == Black){
            if(board[row + 1][col].type == Empty)
                moves.insert({row + 1,col});
            if(col != 7 && board[row + 1][col + 1].type != Empty && board[row + 1][col + 1].color != piece.color)
                moves.insert({row + 1,col + 1});
            if(col && board[row + 1][col - 1].type != Empty && board[row + 1][col - 1].color != piece.color)
                moves.insert({row + 1,col - 1});
            
            if(row == 1)
                moves.insert({row + 2,col});

            if(row == 4){
                if(col && board[row][col - 1].type == Pawn && board[row][col - 1].color == White && lastMoveFrom.first == 6 && lastMoveFrom.second == col - 1){
                    moves.insert({row + 1,col - 1});
                }
                if(col + 1 < 7 && board[row][col + 1].type == Pawn && board[row][col + 1].color == White && lastMoveFrom.first == 6  && lastMoveFrom.second == col + 1){
                    moves.insert({row + 1,col + 1});
                }
            }
        }
    }
    return moves;
}

std::set<std::pair<int,int>>Board::attacked(Color cl){
    std::set<std::pair<int,int>>spots;
    for(int row = 0;row < 8;row++){
        for(int col = 0;col < 8;col++){
            if(board[row][col].type != Empty && board[row][col].color != cl){
                auto current = sudoLegalMoves(board[row][col],0);
                spots.insert(current.begin(),current.end());
            }
        }
    }   
    return spots;
}

bool Board::isChecked(Color cl){
    
    
    if(cl == White){
        auto attackedSpots = attacked(White);
        return attackedSpots.find({WhiteKing.row,WhiteKing.col}) != attackedSpots.end();
        
    }
    else{
        auto attackedSpots = attacked(Black);
        return attackedSpots.find({BlackKing.row,BlackKing.col}) != attackedSpots.end();
        
    }
        
}

void Board::performMove(Piece piece,int row,int col,int movement){

    
    if(piece.type == King){
        if(piece.color == White){
            WhiteKing.row = row;
            WhiteKing.col = col;
        }
        else{
            BlackKing.row = row;
            BlackKing.col = col;
        }
    }
    
    if(piece.type == Pawn && piece.col != col && board[row][col].type == Empty){
        if(piece.color == White){
            board[row + 1][col].type = Empty;
        }
        else{
            board[row - 1][col].type = Empty;
        }
        
    }

    board[piece.row][piece.col].type = Empty;
    board[row][col] = piece;
    board[row][col].row = row;
    board[row][col].col = col;
    if(piece.type == King && std::abs(piece.col - col) > 1){
        if(col == piece.col + 2){
            Piece r;
            r.init(Rook,piece.color,row,col - 1);
            board[r.row][r.col] = r;
            board[r.row][7].type = Empty;
        }
        else{
            Piece r;
            r.init(Rook,piece.color,row,col + 1);
            board[r.row][r.col] = r;
            board[r.row][0].type = Empty;
        }
    }

    
    if(movement && piece.type == Pawn && (row == 7 || row == 0)){
        promote(row,col);
    }
    
}

void Board::reverseMove(Piece from,Piece to){

    if(from.type == King){
        if(from.color == White){
            WhiteKing.row = from.row;
            WhiteKing.col = from.col;
        }
        else{
            BlackKing.row = from.row;
            BlackKing.col = from.col;
        }
    }

    if(from.type == Pawn && from.col != to.col && to.type == Empty){
        if(from.color == White){
            board[to.row + 1][to.col].init(Pawn,Black,to.row - 1,to.col);
        }
        else{
            board[to.row - 1][to.col].init(Pawn,White,to.row + 1,to.col);
        }
    }

    board[from.row][from.col] = from;
    board[to.row][to.col] = to;
    if(from.type == King && std::abs(from.col - to.col) > 1){
        if(to.col == from.col + 2){
            board[to.row][to.col - 1].type = Empty;
            Piece r;
            r.init(Rook,to.color,to.row,7);
            board[r.row][7] = r;
        }
        else{
            board[to.row][to.col + 1].type = Empty;
            Piece r;
            r.init(Rook,to.color,to.row,0);
            board[r.row][0] = r;
        }
    }

    
}



void Board::promote(int row,int col){
    board[row][col].type = MessageBoxManager::getPromotedPiece();
}