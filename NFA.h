#pragma once
#include"Trans.h"
#include<iostream>
#include<vector>
#include<stack>
#include<unordered_set>
class NFA {
public:
	//0 is the only start state
	int vertex_cnt;//Q
	std::vector<Trans> transitions;//delta,transition graph
	std::unordered_set<int> final_states;//F

	NFA(int vertex_cnt = 0) :vertex_cnt(vertex_cnt) {}
	NFA(int vertex_cnt, const std::vector<Trans>& transitions, const std::unordered_set<int>& final_states) :
		vertex_cnt(vertex_cnt), transitions(transitions), final_states(final_states) {}
	int get_vertex_count()const {
		return vertex_cnt;
	}

	void set_vertex_cnt(int vertex_cnt) {
		this->vertex_cnt = vertex_cnt;
	}

	std::vector<Trans> get_transition()const {
		return transitions;
	}

	void add_transition(int vertex_from, int vertex_to, char trans_symbol) {
		transitions.emplace_back(vertex_from, vertex_to, trans_symbol);
	}

	std::unordered_set<int> get_final_states()const {
		return final_states;
	}

	void add_final_state(int state) {
		final_states.insert(state);
	}

	void clear() {
		vertex_cnt = 0;
		transitions.clear();
		final_states.clear();
	}
	
	void display()const {
		printf("-------------------------\n");
		for (Trans temp : transitions) {
			printf("q_%d -> q_%d: %c\n", temp.vertex_from, temp.vertex_to, temp.trans_symbol);
		}
		printf("\n");
		printf("\n");
		printf("final_states: ");
		for (int f : final_states) {
			printf("%d ", f);
		}
		printf("\n");
		printf("-------------------------\n");
		
	}

	std::unordered_set<int> epsilon_closure(const std::unordered_set<int>& T)const {
		std::stack<int> st;
		for (const int& state : T) {
			st.push(state);
		}

		std::unordered_set<int> closure = T;
		while (!st.empty()) {
			int t = st.top();
			st.pop();
			for (const Trans& trans : transitions) {
				if (trans.trans_symbol == EPSILON && 
					closure.find(trans.vertex_from) != closure.end() &&
					closure.find(trans.vertex_to) == closure.end()) {

					closure.insert(trans.vertex_to);
					st.push(trans.vertex_to);
				}
			}
		}

		return closure;
	}

	std::unordered_set<int> move_symbol(const std::unordered_set<int>& T, const char& symbol)const {
		std::unordered_set<int> result;
		for (const int& state : T) {
			for (const Trans& trans : transitions) {
				if (trans.vertex_from == state && trans.trans_symbol == symbol) {
					result.insert(trans.vertex_to);
				}
			}
		}

		return result;
	}

	//u in states, (u,symbol,t) in transition, result={all symbol}
	std::unordered_set<char> find_traverse_symbols(const std::unordered_set<int>& states)const {
		std::unordered_set<char> result;
		for (const int& state : states) {
			for (const Trans& trans : transitions) {
				if (trans.vertex_from == state && trans.trans_symbol != EPSILON) {
					result.insert(trans.trans_symbol);
				}
			}
		}

		return result;
	}
};