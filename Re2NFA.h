#pragma once
#include<iostream>
#include<string>
#include<stack>
#include<cctype>
#include"Trans.h"
#include"NFA.h"

//re: a.b
NFA concat(NFA a, NFA b) {
	int a_vertex_cnt = a.get_vertex_count();
	int b_vertex_cnt = b.get_vertex_count();
	int total_states = a_vertex_cnt + b_vertex_cnt;
	NFA result(total_states, a.get_transition(), { total_states - 1 });
	result.add_transition(a_vertex_cnt - 1, a_vertex_cnt, EPSILON);

	for (const Trans& trans : b.get_transition()) {
		result.add_transition(
			trans.vertex_from + a_vertex_cnt,
			trans.vertex_to + a_vertex_cnt,
			trans.trans_symbol);
	}
	return result;
}

//re: a*
NFA kleene(NFA a) {
	int a_vertex_cnt = a.get_vertex_count();
	NFA result(
		a_vertex_cnt + 2, {
			Trans(0,1,EPSILON),
			Trans(a_vertex_cnt,1,EPSILON),
			Trans(a_vertex_cnt,a_vertex_cnt + 1,EPSILON),
			Trans(0,a_vertex_cnt + 1,EPSILON)
		},
		{ a_vertex_cnt + 1 }
	);

	for (const Trans& trans : a.get_transition()) {
		result.add_transition(trans.vertex_from + 1, trans.vertex_to + 1, trans.trans_symbol);
	}

	return result;
}

//re: a|b
NFA or_selection(NFA a, NFA b) {
	int a_vertex_cnt = a.get_vertex_count();
	int b_vertex_cnt = b.get_vertex_count();

	NFA result(
		a_vertex_cnt + b_vertex_cnt + 2, {
			Trans(0,1,EPSILON),
			Trans(0,a_vertex_cnt + 1,EPSILON),
			Trans(a_vertex_cnt,a_vertex_cnt + b_vertex_cnt + 1,EPSILON),
			Trans(a_vertex_cnt + b_vertex_cnt,a_vertex_cnt + b_vertex_cnt + 1,EPSILON)
		},
		{ a_vertex_cnt + b_vertex_cnt + 1 }
	);

	for (const Trans& trans : a.get_transition()) {
		result.add_transition(trans.vertex_from + 1, trans.vertex_to + 1, trans.trans_symbol);
	}

	for (const Trans& trans : b.get_transition()) {
		result.add_transition(trans.vertex_from + a_vertex_cnt + 1, trans.vertex_to + a_vertex_cnt + 1, trans.trans_symbol);
	}

	return result;
}


int level(const char& op) {
	if (op == '#') {
		return 0;
	}
	else if (op == '(' || op == ')') {
		return 1;
	}
	else if (op == '|') {
		return 2;
	}
	//.
	return 3;
}


/**
 * compare right operator with left operator
 * @param left_operator left operator
 * @param right_operator right operator
 * @return right operator>left operator return 1£¬= return 0£¬< -1
 */
int cmp(const char& left_operator, const char& right_operator) {
	if (left_operator == '(' && right_operator == ')') {
		return 0;
	}
	else if (right_operator == '(') {
		return 1;
	}
	if (level(left_operator) < level(right_operator)) {
		return 1;
	}
	return -1;
}


bool is_operator(const char& op) {
	return op == '|' || op == '.' || op == '*' || op == '(' || op == ')';
}

NFA calculate(const NFA& left_operand, const NFA& right_operand, const char& op) {
	if (op == '|') {
		return or_selection(left_operand, right_operand);
	}
	return concat(left_operand, right_operand);
}


//McNaughton¨CYamada¨CThompson algorithm
NFA re2nfa(const std::string& expression) {
	std::stack<NFA> operands;
	std::stack<char> operators;
	operators.push('#');

	for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
		while (it != expression.end() && isspace(*it)) {
			++it;
		}
		if (it == expression.end()) {
			break;
		}
		char right_op = *it;
		if (is_operator(right_op)) {
			if (right_op == '*') {
				NFA temp = operands.top();
				operands.pop();
				operands.push(kleene(temp));
			}
			else {
				char left_op = operators.top();
				int cmp_result = cmp(left_op, right_op);
				//left_op >= right_op
				while (cmp_result != 1) {
					//left_op=='(' && right_op== ')'
					if (cmp_result == 0) {
						operators.pop();
						break;
					}
					else {
						NFA right_nfa = operands.top();
						operands.pop();
						NFA left_nfa = operands.top();
						operands.pop();
						operands.push(calculate(left_nfa, right_nfa, left_op));

						operators.pop();
						left_op = operators.top();
						cmp_result = cmp(left_op, right_op);
					}
				}
				if (right_op != ')') {
					operators.push(right_op);
				}
			}
		}
		else {
			operands.push(NFA(2, { Trans(0,1,*it) }, { 1 }));
		}
	}
	char op = operators.top();
	while (op != '#') {
		operators.pop();
		NFA right_nfa = operands.top();
		operands.pop();
		if (op == '*') {
			operands.push(kleene(right_nfa));
		}
		else {
			NFA left_nfa = operands.top();
			operands.pop();
			operands.push(calculate(left_nfa, right_nfa, op));
		}
		op = operators.top();
	}
	return operands.top();
}