#pragma once
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<queue>
#include"Trans.h"
#include"NFA.h"
#include"DFA.h"

//subset construction
DFA nfa2dfa(const NFA& nfa) {
	std::vector<std::unordered_set<int> > idx2state;
	std::vector<std::unordered_map<char, int> > transitions;
	
	std::queue<int> q;
	q.push(0);
	idx2state.push_back(nfa.epsilon_closure({ 0 }));
	transitions.emplace_back(std::unordered_map<char, int>());

	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		//for(a in Sigma)
		for (const char& symbol : nfa.find_traverse_symbols(idx2state[cur])) {
			std::unordered_set<int> temp = nfa.epsilon_closure(nfa.move_symbol(idx2state[cur], symbol));
			int i = 0;
			while (i < idx2state.size()) {
				if (idx2state[i] == temp) {
					break;
				}
				++i;
			}
			//mark unseen state
			if (i == idx2state.size()) {
				q.push(i);
				idx2state.emplace_back(temp);
				transitions.emplace_back(std::unordered_map<char, int>());
			}
			transitions[cur][symbol] = i;

		}
	}
	std::unordered_set<int> F = nfa.get_final_states();
	std::unordered_set<int> final_states;
	for (int i = 0; i < idx2state.size(); ++i) {
		for (const int& state : idx2state[i]) {
			//set which contains final is a final set
			if (F.find(state) != F.end()) {
				final_states.insert(i);
				break;
			}
		}
	}
	return DFA(transitions.size(), transitions, final_states);
}