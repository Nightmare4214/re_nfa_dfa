#pragma once
extern char const EPSILON = '$';
class Trans {
public:
	int vertex_from;
	int vertex_to;
	char trans_symbol;
	Trans(int vertex_from = 0, int vertex_to = 1, char trans_symbol = EPSILON) :
		vertex_from(vertex_from), vertex_to(vertex_to), trans_symbol(trans_symbol) {}
};