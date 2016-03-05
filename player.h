#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
	Board * curr_board;
    Side curr_side;
    
    Move *doMove(Move *opponentsMove, int msLeft);
    
	int Heuristic(Move * move);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
