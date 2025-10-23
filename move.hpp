#pragma once

#include "position.hpp"
#include "piece.hpp"
#include "board.hpp"



struct Move {
    Position from;
    Position to;
    Color player_color;

    char move_string[16];

    const char* to_char();
};