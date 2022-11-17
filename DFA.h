#pragma once
#include<vector>
#include<stack>
#include<unordered_map>
#include<unordered_set>
class DFA {
public:
	//0 is the only start state
	int vertex_cnt;//Q
	std::vector<std::unordered_map<char, int> > transitions;//delta,transition graph
	std::unordered_set<int> final_states;//F
	DFA(int vertex_cnt = 0) :vertex_cnt(vertex_cnt) {}
	DFA(int vertex_cnt, const std::vector<std::unordered_map<char, int> >& transitions, const std::unordered_set<int>& final_states) :
		vertex_cnt(vertex_cnt), transitions(transitions), final_states(final_states) {}

	int get_vertex_cnt()const {
		return vertex_cnt;
	}

	std::vector<std::unordered_map<char, int> > get_transitions()const {
		return transitions;
	}

	std::unordered_set<int> get_final_states()const {
		return final_states;
	}

	void display()const {
		printf("-------------------------\n");
		for (int i = 0; i < transitions.size(); ++i) {
			for (const auto& temp : transitions[i]) {
				printf("q_%d -> q_%d: %c\n", i, temp.second, temp.first);
			}
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
};