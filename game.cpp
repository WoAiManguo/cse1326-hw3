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
    
    return BLACK;
}

bool Game::isGameOver() const {
    if (board.getRedCount() == 0 || board.getBlackCount() == 0) {
        return true;
    }
    return false;
}

bool Game::handlePlayerTurn() {
    Move m;
    strcpy(error_message, "No error.");
    
    while (true) {
        io->displayBoard(board, "Your turn (BLACK).", error_message);
        
        if (!io->getPlayerMove(current_player, &m)) {
            strcpy(error_message, "Invalid input format or position. Use 'C3 to D4'.");
            continue;
        }

        if (m.from.row == -1) { 
            return false;
        }

        MoveError error = board.isLegalMove(m);
        if (error == NO_ERROR) {
            if (board.movePiece(m)) {
                move_history.push_back(m);
                return true;
            } else {
                strcpy(error_message, "Internal error during move execution.");
                continue;
            }
        } else {
            m.to_char();
            sprintf(error_message, "Move %s is illegal (Error %d). Try again.", m.move_string, error);
            continue;
        }
    }
}

bool Game::handleMachineTurn() {
    strcpy(error_message, "No error.");
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position from = {r, c};
            const Piece* piece = board.getPiece(from);

            if (piece != NULL && piece->getColor() == current_player) {
                for (int dr_sign = -1; dr_sign <= 1; dr_sign += 2) {
                    for (int dc_sign = -1; dc_sign <= 1; dc_sign += 2) {
                        
                        int dr1 = dr_sign * 1;
                        int dc1 = dc_sign * 1;
                        Move m1 = {from, {r + dr1, c + dc1}, current_player};

                        if (m1.to.is_valid() && board.isLegalMove(m1) == NO_ERROR) {
                            if (board.movePiece(m1)) {
                                move_history.push_back(m1);
                                return true; 
                            }
                        }

                        int dr2 = dr_sign * 2;
                        int dc2 = dc_sign * 2;
                        Move m2 = {from, {r + dr2, c + dc2}, current_player};
                        
                        if (m2.to.is_valid() && board.isLegalMove(m2) == NO_ERROR) {
                            if (board.movePiece(m2)) {
                                move_history.push_back(m2);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    strcpy(error_message, "Machine (RED) has no legal moves. Game Over.");
    game_over = true;
    return true; 
}

void Game::run() {
    initializeGame();
    printf("Game started. BLACK (bottom) moves first. Type 'stop' to exit.\n");

    while (!game_over) {
        bool continue_game = true;
        
        if (current_player == RED) {
            io->displayBoard(board, "Machine's turn (RED).", error_message);
        }

        if (current_player == BLACK) {
            continue_game = handlePlayerTurn();
        } else {
            continue_game = handleMachineTurn();
        }

        if (!continue_game) {
            printf("\nUser entered 'stop'. Exiting game.\n");
            break;
        }
        
        if (game_over) {
            continue;
        }
        
        if (isGameOver()) {
            game_over = true;
            continue;
        }

        current_player = (current_player == BLACK) ? RED : BLACK;
    }

    io->displayBoard(board, "Game Over!", error_message);
    
    if (board.getRedCount() == 0) {
        printf("\n*** GAME OVER: BLACK Wins! ***\n");
    } else if (board.getBlackCount() == 0) {
        printf("\n*** GAME OVER: RED Wins! ***\n");
    } else {
        printf("\n*** GAME ENDED EARLY ***\n");
    }

    io->displayMoveHistory(move_history);
}