
#include "pieces.h"
#include "move_checker.h"
#include "board_state.h"
#include "move.h"

#include <stdlib.h>

struct move_checker_t * new_move_checker(int board[6][6]) {
	struct move_checker_t * this = malloc(sizeof(struct move_checker_t));
	
	if(!this)
		return NULL;
	
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			this->board[i][j] = board[i][j];
	
	this->white_short_castling = true;
	this->black_short_castling = true;
	
	this->white_long_castling = true;
	this->black_long_castling = true;
	
	this->last_player_moved = 1;
	this->last_move = new_move();
	
	this->kingrw = 4;
	this->kingrb = 4;
	
	for(int i = 0; i < 6; i++)
		if(board[i][0] == WHITE_KING)
			this->kingrw = i;
	
	for(int i = 0; i < 6; i++)
		if(board[i][5] == BLACK_KING)
			this->kingrb = i;
	
	return this;
}

struct move_checker_t * new_move_checker_clone(struct move_checker_t * clone) {
	struct move_checker_t * this = malloc(sizeof(struct move_checker_t));
	
	if(!this)
		return NULL;
	
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			this->board[i][j] = clone->board[i][j];
	
	this->white_short_castling = clone->white_short_castling;
	this->black_short_castling = clone->black_short_castling;
	
	this->white_long_castling = clone->white_long_castling;
	this->black_long_castling = clone->black_long_castling;
	
	this->last_player_moved = clone->last_player_moved;
	this->last_move = new_move_coord(
		clone->last_move->start_col, clone->last_move->start_row,
		clone->last_move->end_col, clone->last_move->end_row
	);
	
	return this;
}

bool move_checker_legal_square(struct move_checker_t * this, int col, int row) {
	return col > 0 && col < 6 && row > 0 && row < 6 && this->board[col][row] != UNDEFINED;
}

bool move_checker_legal_move(struct move_checker_t * this, int p, int x1, int y1, int x2, int y2) {
	return move_checker_semi_legal_move(this, x1, y1, x2, y2)
	&& move_checker_in_check_after_move(this, p, x1, y1, x2, y2);
}

bool move_checker_legal_move_m(struct move_checker_t * this, int p, struct move_t * m) {
	return move_checker_legal_move(this, p, m->start_col, m->start_row, m->end_col, m->end_row);
}

