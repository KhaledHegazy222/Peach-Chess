#pragma once	

#include "Board.hpp"

class Engine {

private:

public:
	Engine();
	int search(int depth,Board& board,int alpha,int beta,Color turn,bool firstLevel);
	int evaluate(Board& board);
	std::pair<Position, Position> getBest(Board& board);

};