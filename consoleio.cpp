#include "consoleio.hpp"
#include "board.hpp"
#include <cstdio>
#include <cstring>

void ConsoleIO::displayBoard(const Board& board, const char* status_msg, const char* error_msg) {
    printf("ERROR: %s\n", error_msg);
    printf("STATUS: %s\n", status_msg);

    for (int r = 7; r >= 0; --r) {
        printf("%d |", r + 1);
        for (int c = 0; c < 8; ++c) {
            Position pos = {r, c};
            const Piece* p = board.getPiece(pos);
            char display_char = p ? p->getSymbol() : ' ';
            printf(" %c |", display_char);
        }
        printf(" %d\n", r + 1);
    }

    
    printf("  +---+---+---+---+---+---+---+---+\n");
    printf("    A   B   C   D   E   F   G   H\n");
}

bool ConsoleIO::getPlayerMove(Color player, Move* move_out) {
    char from_str[3] = {'\0'};
    char to_str[3] = {'\0'};
    char to_keyword[3] = {'\0'};
    char stop_cmd[5] = {'\0'};

    printf("\n%s Player, enter move (e.g., C3 to D4) or 'stop': ", (player == BLACK) ? "BLACK" : "RED");

    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    
    if (scanf("%4s", stop_cmd) == 1 && strcmp(stop_cmd, "stop") == 0) {
        move_out->from = {-1, -1};
        return true;
    }
    
    
    
    if (sscanf(stop_cmd, "%2s", from_str) == 1 && 
        fscanf(stdin, "%2s %2s", to_keyword, to_str) == 2 && 
        strcmp(to_keyword, "to") == 0) 
    {
        
        move_out->from = Position::str_to_ind(from_str);
        move_out->to = Position::str_to_ind(to_str);
        move_out->player_color = player;

        if (move_out->from.is_valid() && move_out->to.is_valid()) {
            return true;
        }
    }

    return false;
}

void ConsoleIO::displayMoveHistory(const std::vector<Move>& history) {
    printf("\n--- Move History ---\n");
    if (history.empty()) {
        printf("No moves were made.\n");
        return;
    }

    for (size_t i = 0; i < history.size(); ++i) {
        
        printf("%zu. %s\n", i + 1, history[i].to_char());
    }
    printf("--------------------\n");
}