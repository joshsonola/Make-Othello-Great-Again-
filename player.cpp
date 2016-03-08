#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;
    
    curr_board = new Board();
    curr_side = side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
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

    // If there is only one possible move, then make that move
	if (possible_moves.size() == 1) {
		final_move = possible_moves[0];
	}
    else {
		if (testingMinimax) {
			// Use the Minimax procedure to find a move to make
			final_move = MiniMax(possible_moves);	
		}
		else {
			// Use the Heuristic procedure to find a move to make
			final_move = Heuristic(possible_moves, curr_side, curr_board);
		}
	}
	
	// Perform our move
    curr_board->doMove(final_move, curr_side);
    // Return the move
	return final_move;
}

Move * Player::MiniMax(std::vector<Move *> possible_moves) {
	unsigned int min_index = 0;
	
    // Initialize min_score (perform same function on first move) and score
    int min_score = MiniMaxValue(possible_moves[0]);
	int score;
	
    // For each possible move that we can make, get the score of the best move
    // our opponent can make in response
    for (unsigned int i = 1; i < possible_moves.size(); i++) {
		score = MiniMaxValue(possible_moves[i]);
		
        // If this score is less than min_score, update the min_score and
        // min_index
        if (score < min_score) {
			min_index = i;
			min_score = score;
		}
	}
	
    // Return the move that maximizes our minimum gain
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
			if (copy_board->checkMove(&move, other)) {
				minimax_possible_moves.push_back(new Move(i, j));
			}       
		}
	}
	
    // Initialize the return value
    int ret_value;

    // Get the move that maximizes the opponent's score on the copy of the board
	Move * MiniMax_move = Heuristic(minimax_possible_moves, other, copy_board);
	// Get the score of that move
    ret_value = HeuristicValue(MiniMax_move, other, copy_board);

	// Delete memory allocated for the copy of the board
    delete copy_board;

    // Return the return value (score of the opponent's best move)
	return ret_value;
	
}

Move * Player::Heuristic(std::vector<Move *> heur_moves, Side side, Board * board) {
    // If no moves, return NULL
    if (heur_moves.size() == 0) {
		return NULL;
	}
	
    // Initialize max_score (same function on first move), max_index, score
    int max_score = HeuristicValue(heur_moves[0], side, board);
    unsigned int max_index = 0;
    int score;
    
    // For each possible move, get the score of the move
    for (unsigned int i = 1; i < heur_moves.size(); i++) {
		score = HeuristicValue(heur_moves[i], side, board);
		
        // If this score is greater than the max_score, then update the 
        // max_score and max_index (index of the max_score)
        if (score > max_score) {
			max_index = i;
			max_score = score;
		}
	}
	
    // Return the move with the maximum score
    return heur_moves[max_index];
}

int Player::HeuristicValue(Move * curr_move, Side side, Board * board) {
	// Perform the move on a copy of the game board to determine the 
	// score
	Board * copy_board = board->copy();
	copy_board->doMove(curr_move, side);
	
	// Obtain the opponent's side
    Side other = (side == BLACK) ? WHITE : BLACK;
    
    // The score is simply the move that results in more pieces than 
    // the opponent
	int score = copy_board->count(side) - copy_board->count(other);
	
	if (curr_move != NULL) {
		// Store variables for current moves
		int curr_x = curr_move->getX();
		int curr_y = curr_move->getY();
		
		// Multiply by a modifier based on the location of the move

        // If it is a corner, multiply the score by 3
		if ((curr_x == 0 && curr_y == 0) || (curr_x == 0 && curr_y == 7) ||
			(curr_x == 7 && curr_y == 0) || (curr_x == 7 && curr_y == 7)) 
		{
			score *= 3;
		}
		
        // If it is adjacent to a corner, multiply the score by -3
        else if ((curr_x == 1 && curr_y == 0) || (curr_x == 0 && curr_y == 1) ||  
			(curr_x == 0 && curr_y == 6) || (curr_x == 1 && curr_y == 7) || 
			(curr_x == 7 && curr_y == 1) || (curr_x == 6 && curr_y == 0) ||
			(curr_x == 6 && curr_y == 7) || (curr_x == 7 && curr_y == 6) ||
			(curr_x == 1 && curr_y == 1) || (curr_x == 6 && curr_y == 6) || 
			(curr_x == 1 && curr_y == 6) || (curr_x == 6 && curr_y == 1)) 
		{
			score *= -3;
		}	
		
        

        // If it is an edge piece (other than a corner or adjacent to a corner),
        // multiply the score by 2.
        /*
        else if (curr_x == 0 || curr_x == 7 || curr_y == 0 || curr_y == 7)
        (curr_x == 2 && (curr_y != 3 && curr_y != 4)) || 
		(curr_x == 5 && (curr_y != 3 && curr_y != 4)) || 
		(curr_y == 2 && (curr_x != 3 && curr_x != 4)) || 
		(curr_y == 5 && (curr_x != 3 && curr_x != 4)))
        
		{
			score *= 2;
		}
        */
	}
	
    // Delete memory allocated to the copy of the board
    delete copy_board;
	
    // Return the score
    return score;
}
