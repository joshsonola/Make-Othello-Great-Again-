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
    if (testingMinimax) {
		// Use the Minimax procedure to find a move to make
		final_move = MiniMax(possible_moves);		
	}
	else {
		// Use the Heuristic procedure to find a move to make
		final_move = Heuristic(possible_moves, curr_side);
	}
	
    // Delete possible_moves
    for (unsigned int i = 0; i < possible_moves.size(); i++)
    {
        //delete possible_moves[i];
    }
	
	curr_board->doMove(final_move, curr_side);
	return final_move;
}

Move * Player::MiniMax(std::vector<Move *> possible_moves) {
	unsigned int min_index = 0;
	int min_score = MiniMaxValue(possible_moves[0]);
	int score;
	for (unsigned int i = 1; i < possible_moves.size(); i++) {
		score = MiniMaxValue(possible_moves[i]);
		if (score < min_score) {
			min_index = i;
			min_score = score;
		}
	}
	return possible_moves[min_index];
}

int Player::MiniMaxValue(Move * curr_move) {
	// Perform the move on a copy of the game board to determine the 
	// minimax score
	Board * copy_board = curr_board->copy();
	copy_board->doMove(curr_move, curr_side);
	
	// Obtain the opponents 
    Side other = (curr_side == BLACK) ? WHITE : BLACK;
    
	// Obtain a vector of all possible moves
	std::vector<Move *> minimax_possible_moves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Move move(i, j);
			if (curr_board->checkMove(&move, other)) {
				minimax_possible_moves.push_back(new Move(i, j));
			}       
		}
	}
	Move * final_move = Heuristic(minimax_possible_moves, other);
    
    // Delete moves in all possible moves vector
    for (unsigned int i = 0; i < minimax_possible_moves.size(); i++)
    {
        //delete minimax_possible_moves[i];
    }

	delete copy_board;
	return HeuristicValue(final_move, other);
}

Move * Player::Heuristic(std::vector<Move *> possible_moves, Side side) {
    if (possible_moves.size() == 0) {
		return NULL;
	}
	int max_score = HeuristicValue(possible_moves[0], side);
    unsigned int max_index = 0;
    int score;
    for (unsigned int i = 1; i < possible_moves.size(); i++) {
		score = HeuristicValue(possible_moves[i], side);
		if (score > max_score) {
			max_index = i;
			max_score = score;
		}
	}
	return possible_moves[max_index];
}

int Player::HeuristicValue(Move * curr_move, Side side) {
	// Perform the move on a copy of the game board to determine the 
	// score
	Board * copy_board = curr_board->copy();
	copy_board->doMove(curr_move, curr_side);
	
	// Obtain the opponents side
    Side other = (side == BLACK) ? WHITE : BLACK;
    
    // The score is simply the move that results in more pieces than 
    // the opponent
	int score = copy_board->count(side) - copy_board->count(other);
	
	// Store variables for current moves
	int curr_x = curr_move->getX();
	int curr_y = curr_move->getY();
	
	// Multiply by a modifier based on the location of the move
	if ((curr_x == 0 && curr_y == 0) || (curr_x == 0 && curr_y == 7) ||
		(curr_x == 7 && curr_y == 0) || (curr_x == 7 && curr_y == 7)) 
	{
		
		score *= 3;
	}
	else if ((curr_x == 1 && curr_y == 0) || (curr_x == 0 && curr_y == 1) ||  
		(curr_x == 0 && curr_y == 6) || (curr_x == 1 && curr_y == 7) || 
		(curr_x == 7 && curr_y == 1) || (curr_x == 6 && curr_y == 0) ||
		(curr_x == 6 && curr_y == 7) || (curr_x == 7 && curr_y == 6) ||
		(curr_x == 1 && curr_y == 1) || (curr_x == 6 && curr_y == 6) || 
		(curr_x == 1 && curr_y == 6) || (curr_x == 6 && curr_y == 1)) 
	{
		score *= -3;
	}	
	else if (curr_x == 0 || curr_x == 7 || curr_y == 0 || curr_y == 7 ||
	(curr_x == 2 && (curr_y != 3 && curr_y != 4)) || 
	(curr_x == 5 && (curr_y != 3 && curr_y != 4)) || 
	(curr_y == 2 && (curr_x != 3 && curr_x != 4)) || 
	(curr_y == 5 && (curr_x != 3 && curr_x != 4)))
	{
		score *= 2;
	}
	delete copy_board;
	return score;
}
