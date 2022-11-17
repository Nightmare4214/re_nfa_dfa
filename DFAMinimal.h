#pragma once
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<vector>
#include<algorithm>
#include"DFA.h"

//bfs
DFA remove_unreachable_state(const DFA& dfa, const std::vector<char>& symbols) {
	std::queue<int> q;
	q.push(0);
	std::unordered_set<int> reachable_state = { 0 };
	int vertex_cnt = dfa.get_vertex_cnt();
	std::vector<std::unordered_map<char, int> > transitions = dfa.get_transitions();
	std::unordered_set<int> final_states = dfa.get_final_states();
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		std::unordered_map<char, int> temp = transitions[cur];
		//traverse symbols (Sigma)
		for (const char& c : symbols) {
			if (temp.find(c) != temp.end()) {
				int state = temp[c];
				if (reachable_state.find(state) == reachable_state.end()) {
					q.push(state);
					reachable_state.insert(state);
				}
			}
		}
		
	}

	std::vector<int> idx2state(vertex_cnt);
	std::vector<std::unordered_map<char, int> > new_transitions;
	std::unordered_set<int> new_final_states;
	int cnt = 0;
	//relabel and find new final states
	for (int i = 0; i < vertex_cnt; ++i) {
		if (reachable_state.find(i) != reachable_state.end()) {
			idx2state[i] = cnt;
			if (final_states.find(i) != final_states.end()) {
				new_final_states.insert(cnt);
			}
			++cnt;
		}
	}

	for (int i = 0; i < vertex_cnt; ++i) {
		if (reachable_state.find(i) != reachable_state.end()) {
			std::unordered_map<char, int> temp;
			for (const auto& transition : transitions[i]) {
				if (reachable_state.find(transition.second) != reachable_state.end()) {
					temp[transition.first] = idx2state[transition.second];
				}
			}
			new_transitions.push_back(temp);
		}
	}
	return DFA(cnt, new_transitions, new_final_states);
}

DFA hopcroft(const DFA& dfa, const std::vector<char>& symbols) {
	int vertex_cnt = dfa.get_vertex_cnt();
	std::vector<std::unordered_map<char, int> > transitions = dfa.get_transitions();
	std::unordered_set<int> final_states = dfa.get_final_states();

	std::unordered_set<int> start_states;
	for (int i = 0; i < vertex_cnt; ++i) {
		if (final_states.find(i) == final_states.end()) {
			start_states.insert(i);
		}
	}

	std::vector<std::unordered_set<int> > partition = { start_states,final_states };//Q'
	std::vector<std::unordered_set<int> > work_list = { final_states };//W
	while (!work_list.empty()) {
		std::unordered_set<int> q_prime = work_list.back();
		work_list.pop_back();

		for (const char& symbol : symbols) {
			std::unordered_set<int> x;//predecessor
			for (int i = 0; i < vertex_cnt; ++i) {
				if (transitions[i].find(symbol) != transitions[i].end()&&
					q_prime.find(transitions[i][symbol]) != q_prime.end()) {
					x.insert(i);
				}
			}

			if (!x.empty()) {
				std::vector<std::unordered_set<int> > temp;
				for (const auto& y : partition) {
					std::unordered_set<int> y_cap_x;//y cap x
					std::unordered_set<int> y_diff_x;//y-x
					//split
					for (const int& state : y) {
						if (x.find(state) != x.end()) {
							y_cap_x.insert(state);
						}
						else {
							y_diff_x.insert(state);
						}
					}

					if (!y_cap_x.empty() && !y_diff_x.empty()) {
						temp.push_back(y_cap_x);
						temp.push_back(y_diff_x);

						auto it = std::find(work_list.begin(), work_list.end(), y);
						if (it != work_list.end()) {
							work_list.erase(it);
							work_list.push_back(y_cap_x);
							work_list.push_back(y_diff_x);
						}
						else if (y_cap_x.size() < y_diff_x.size()) {
							work_list.push_back(y_cap_x);
						}
						else {
							work_list.push_back(y_diff_x);
						}
					}
					else {
						//indistinguish, don't split y
						temp.push_back(y);
					}
				}
				partition = temp;
			}
		}

	}

	int cnt = partition.size();
	//let the partition which contains 0 also be 0 in the new dfa
	for (int i = 0; i < cnt; ++i) {
		bool flag = false;
		for (const int& state : partition[i]) {
			if (state == 0) {
				std::swap(partition[i], partition[0]);
				flag = true;
				break;
			}
		}
		if (flag) {
			break;
		}
	}

	std::vector<std::unordered_map<char, int> > new_transitions(cnt);
	std::unordered_set<int> new_final_states;
	std::vector<int> idx2state(vertex_cnt);
	
	//relabel and find new final states
	for (int i = 0; i < cnt; ++i) {
		for (const int& state : partition[i]) {
			idx2state[state] = i;
			if (final_states.find(state) != final_states.end()) {
				new_final_states.insert(i);
			}

		}
	}

	for (int i = 0; i < transitions.size(); ++i) {
		for (const auto& p : transitions[i]) {
			new_transitions[idx2state[i]][p.first] = idx2state[p.second];
		}
	}

	return DFA(cnt, new_transitions, new_final_states);
}