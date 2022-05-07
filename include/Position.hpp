#pragma once
class Position {
public:


	int row;
	int column;

	Position() {
		row = -1;
		column = -1;
	}
	Position(int _row, int _column) {
		row = _row;
		column = _column;
	}
	void init(int _row, int _column) {
		row = _row;
		column = _column;
	}
	Position operator= (const Position& other) {
		row = other.row;
		column = other.column;
		return *this;
	}
	bool operator< (const Position& other) const {
		return (row * 8 + column) < (other.row * 8 + other.column);
	}
};

