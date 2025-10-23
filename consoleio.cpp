#include "consoleio.hpp"
#include "board.hpp"
#include <cstdio>
#include <cstring>

void ConsoleIO::displayBoard(const Board& board, const char* status_msg, const char* error_msg) {
    printf("ERROR: %s\n", error_msg);
    printf("STATUS: %s\n", status_msg);

    for (int r = 7; r >= 0; --r) {
        printf("%d |", r + 1); // Row number (8 to 1)
        for (int c = 0; c < 8; ++c) {
            Position pos = {r, c};
            const Piece* p = board.getPiece(pos);
            char display_char = p ? p->getSymbol() : ' ';
            printf("%c ", display_char);
        }
        printf("| %d\n", r + 1);
    }

    printf("+---+---+---+---+---+---+---+---+");
    printf("  A   B   C   D   E   F   G   H  ");
}

bool ConsoleIO::getPlayerMove(Color player, Move* move_out) {
    char input_buffer[100];
    char from_str[3] = {'\0'};
    char to_str[3] = {'\0'};
    char to_keyword[3] = {'\0'};

    printf("\n%s Player, enter move (e.g., C3 to D4) or 'stop': ", (player == BLACK) ? "BLACK" : "RED");

    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return false; 
    }

    if (strcmp(input_buffer, "stop\n") == 0) {
        move_out->from = {-1, -1};
        return true;
    }

    if (sscanf(input_buffer, "%2s %2s %2s", from_str, to_keyword, to_str) == 3 && strcmp(to_keyword, "to") == 0) {
        move_out->from = Position::from_char_ptr(from_str);
        move_out->to = Position::from_char_ptr(to_str);
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
        printf("%zu. %s\n", i + 1, history[i].to_char_ptr());
    }
    printf("--------------------\n");
}
