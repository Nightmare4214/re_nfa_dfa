#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include"NFA2RE.h"
#include"DFA.h"

std::string dfa2re(const DFA& dfa) {
	int cnt = dfa.get_vertex_cnt() + 2;
	std::vector<std::unordered_map<char, int> > transitions = dfa.get_transitions();
	std::vector<std::unordered_map<int, std::string> > edges(cnt);
	edges[0][1] = std::string(1, EPSILON);
	for (int u = 0; u < transitions.size(); ++u) {
		for (const auto& p : transitions[u]) {
			merge_edge(edges[u + 1], p.second + 1, std::string(1, p.first));
		}
		
	}

	for (const int& state : dfa.get_final_states()) {
		edges[state + 1][cnt - 1] = std::string(1, EPSILON);
	}

	//remove states except the start and final
	remove_states(edges);

	return edges[0][cnt - 1];
}