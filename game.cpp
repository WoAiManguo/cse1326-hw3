#include "game.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>

Game::Game(GameIO* game_io) 
    : io(game_io), current_player(BLACK), game_over(false) {
    board.initialize();
    strcpy(error_message, "No error.");
}

void Game::initializeGame() {
    current_player = BLACK; 
    game_over = false;
    move_history.clear();
}

Color Game::determineWinner() const {
    if (board.getRedCount() == 0) return BLACK;
    if (board.getBlackCount() == 0) return RED;
    // TBD: Add check for stalemate (no legal moves left)
    return BLACK;
}

bool Game::isGameOver() const {
    if (board.getRedCount() == 0 || board.getBlackCount() == 0) {
        return true;
    }
    // TBD: Check if a player has no legal moves
    return false;
}

bool Game::handlePlayerTurn() {
    Move m;
    strcpy(error_message, "No error.");
    
    while (true) {
        io->displayBoard(board, "Your turn (BLACK).", error_message);
        
        if (!io->getPlayerMove(current_player, &m)) {
            strcpy(error_message, "Invalid input format. Use 'C3 to D4'.");
            continue;
        }

        if (m.from.row == -1) {
            return false;
        }

        if (board.isLegalMove(m)) {
            if (board.movePiece(m)) {
                move_history.push_back(m);
                return true;
            } else {
                strcpy(error_message, "Internal error during move execution.");
                continue;
            }
        } else {
            m.to_char();
            sprintf(error_message, "Move %s is illegal. Try again.", m.move_string);
            continue;
        }
    }
}

bool Game::handleMachineTurn() {
    // TBD: Implement simple machine logic here. 
    // For now, it just passes the turn to avoid an infinite loop if called.
    
    // In a final version, this would look like:
    // 1. Generate all legal moves (board.getLegalMoves(RED))
    // 2. Select one (e.g., the first one, or a random one)
    // 3. Execute it (board.movePiece(m))
    // 4. Record it (move_history.push_back(m))
    
    // Simple placeholder to pass the turn back immediately
    strcpy(error_message, "Machine turn skipped (TBD: Implement simple AI).");
    return true; 
}

void Game::run() {
    initializeGame();
    printf("Game started. BLACK (bottom) moves first. Type 'stop' to exit.\n");

    while (!game_over) {
        bool continue_game = true;

        if (current_player == BLACK) {
            continue_game = handlePlayerTurn();
        } else {
            continue_game = handleMachineTurn();
        }

        if (!continue_game) {
            printf("\nUser entered 'stop'. Exiting game.\n");
            break;
        }
        
        if (isGameOver()) {
            game_over = true;
            continue;
        }

        current_player = (current_player == BLACK) ? RED : BLACK;
    }

    io->displayBoard(board, "Game Over!", error_message);
    
    Color winner = determineWinner();
    if (board.getRedCount() == 0) {
        printf("\n*** GAME OVER: BLACK Wins! ***\n");
    } else if (board.getBlackCount() == 0) {
        printf("\n*** GAME OVER: RED Wins! ***\n");
    } else {
        printf("\n*** GAME ENDED EARLY ***\n");
    }

    io->displayMoveHistory(move_history);
}
