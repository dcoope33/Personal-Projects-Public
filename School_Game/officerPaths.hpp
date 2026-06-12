#include "background.hpp"

std::vector<Uint32> path1 = {
    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_RIGHT,  2000, // (3000 - 1000) = 2000ms spent looking left
    IDLE,       WALK_UP,    1000,  // (4000 - 3000) = 1000ms spent looking up

    WALK_LEFT,  WALK_LEFT,  7000,

    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_LEFT, 2000, // (3000 - 1000) = 2000ms spent looking right
    IDLE,       WALK_UP,    1000, // (4000 - 3000) = 1000ms spent looking up

    WALK_RIGHT, WALK_RIGHT, 7000
};

std::vector<Uint32> path2 = {
    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_RIGHT,  2000, // (3000 - 1000) = 2000ms spent looking left
    IDLE,       WALK_UP,    1000,  // (4000 - 3000) = 1000ms spent looking up

    WALK_LEFT,  WALK_LEFT,  8000,

    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_LEFT, 2000, // (3000 - 1000) = 2000ms spent looking right
    IDLE,       WALK_UP,    1000, // (4000 - 3000) = 1000ms spent looking up

    WALK_RIGHT, WALK_RIGHT, 8000
};

std::vector<Uint32> path3 = {
    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_RIGHT,  2000, // (3000 - 1000) = 2000ms spent looking left
    IDLE,       WALK_UP,    1000,  // (4000 - 3000) = 1000ms spent looking up

    WALK_LEFT,  WALK_LEFT,  3500,

    WALK_UP, WALK_UP, 700,

    WALK_LEFT,  WALK_LEFT,  4500,

    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_LEFT, 2000, // (3000 - 1000) = 2000ms spent looking right
    IDLE,       WALK_UP,    1000, // (4000 - 3000) = 1000ms spent looking up

    WALK_RIGHT, WALK_RIGHT, 4500,

    WALK_DOWN, WALK_DOWN, 700,

    WALK_RIGHT, WALK_RIGHT, 3500
};

std::vector<Uint32> path4 = {
    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_RIGHT,  2000, // (3000 - 1000) = 2000ms spent looking left
    IDLE,       WALK_UP,    1000,  // (4000 - 3000) = 1000ms spent looking up

    WALK_LEFT,  WALK_LEFT,  4800,

    WALK_UP, WALK_UP, 600,

    WALK_LEFT,  WALK_LEFT,  3200,

    IDLE,       WALK_DOWN,  1000,
    IDLE,       WALK_LEFT, 2000, // (3000 - 1000) = 2000ms spent looking right
    IDLE,       WALK_UP,    1000, // (4000 - 3000) = 1000ms spent looking up

    WALK_RIGHT, WALK_RIGHT, 3200,

    WALK_DOWN, WALK_DOWN, 600,

    WALK_RIGHT, WALK_RIGHT, 4800
};