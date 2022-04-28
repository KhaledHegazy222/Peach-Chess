#include "MessageBoxManager.hpp"
#include "Game.hpp"



PieceType MessageBoxManager::getPromotedPiece(){
    
     const SDL_MessageBoxButtonData buttons[] = {
        /* .flags, .buttonid, .text */        
        { 0, 0, "Queen"},
        { 0, 1, "Knight"},
        { 0, 2, "Bishop"},
        { 0, 3, "Rook"},
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Promotion", /* .title */
        "Select Promotion Piece", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return Empty;
    }
    if (buttonid == -1) 
        return Empty;
    else if(buttonid == 0)
        return Queen;
    else if(buttonid == 1)
        return Knight;
    else if(buttonid == 2)
        return Bishop;
    else if(buttonid == 3)
        return Rook;
    

}