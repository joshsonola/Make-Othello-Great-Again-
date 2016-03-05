#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     curr_board = new Board();
     curr_side = side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	// Mojo was here
    // vinci was also here
	delete curr_board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
     
    // Obtain the opponents side
    Side other = (curr_side == BLACK) ? WHITE : BLACK;
    // Update the board with the opponent's move
    curr_board->doMove(opponentsMove, other);
    
    // If there are no possible moves, return NULL
    if (!curr_board->hasMoves(curr_side))
		return NULL;
    
    // Obtain a vector of all possible moves
    std::vector<Move *> possible_moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (curr_board->checkMove(&move, curr_side)) {
				possible_moves.push_back(new Move(i, j));
			}       
        }
    }
    
    Move * final_move;
    
    int max_score = Heuristic(possible_moves[0]);
    unsigned int max_index = 0;
    int score;
    for (unsigned int i = 1; i < possible_moves.size(); i++) {
		score = Heuristic(possible_moves[i]);
		if (score > max_score) {
			max_index = i;
		}
	}
	final_move = possible_moves[max_index];
    // Still need to delete all the moves in possible_moves vector
    curr_board->doMove(final_move, curr_side);
    return final_move;
}

int Player::Heuristic(Move * curr_move) {
	// Perform the move on a copy of the game board to determine the 
	// score
	Board * copy_board = curr_board->copy();
	copy_board->doMove(curr_move, curr_side);
	
	// Obtain the opponents side
    Side other = (curr_side == BLACK) ? WHITE : BLACK;
    
    // The score is simply the move that results in more pieces than 
    // the opponent
	int score = copy_board->count(curr_side) - copy_board->count(other);
	
	// Store variables for current moves
	curr_x = curr_move->getX();
	curr_y = curr_move->getY();
	
	// Multiply by a modifier based on the location of the move
	Move corner_moves[] = {Move(0, 0), Move(0, 7), Move(7, 0), Move(7, 7)}; 
	Move adjacent_corner_moves[] = {Move(0, 1), Move(1, 0), Move(1, 7), 
		Move(0, 6), Move(6, 0), Move(7, 1), Move(6, 7), Move( 7, 6)}; 
	

	if ((curr_x == 0 && curr_y == 0) && (curr_x == 0 && curr_y == 7) &&
		(curr_x == 7 && curr_y == 0) && (curr_x == 7 && curr_y == 7)) 
	{
			score *= 3;
	}
	else if ((curr_x == 0 && curr_y == 0) && (curr_x == 0 && curr_y == 7) &&
		(curr_x == 7 && curr_y == 0) && (curr_x == 7 && curr_y == 7)) 
	{
			score *= -3;
	}	
	delete copy_board;
	return score;
}
