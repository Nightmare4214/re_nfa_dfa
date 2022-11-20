#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include"Trans.h"
#include"NFA.h"

void merge_edge(std::unordered_map<int, std::string>& edges, int u, const std::string& re) {
	if (edges.find(u) == edges.end() || edges[u].empty()) {
		edges[u] = re;
	}
	else {
		edges[u] = "(" + edges[u] + ")|(" + re + ")";
	}
}

std::string merge_string(const std::string& left, const std::string& right) {
	std::string result = left;
	if (left.empty() || left == "$") {
		result = right;
	}
	else if (!right.empty() && right != "$") {
		result = "(" + result + ").(" + right + ")";
	}

	return result;
}

std::string merge_string(const std::vector<std::string>& strs) {
	std::string result;
	for (const std::string& s : strs) {
		result = merge_string(result, s);
	}
	return result;
}

void remove_states(std::vector<std::unordered_map<int, std::string> >& edges) {
	int cnt = edges.size();

	//remove states except the start and final
	for (int i = 1; i + 1 < cnt; ++i) {//remove i
		std::string mid;
		if (edges[i].find(i) != edges[i].end()) {
			mid = "(" + edges[i][i] + ")*";
		}
		edges[i].erase(i);

		for (int j = 0; j < cnt; ++j) {
			if (0 < j && j <= i) {
				continue;
			}
			//j->i
			if (edges[j].find(i) != edges[j].end()) {
				std::string left = edges[j][i];
				edges[j].erase(i);
				//j->i->k
				for (const auto& p : edges[i]) {
					//j.i*.k
					merge_edge(edges[j], p.first, merge_string({ left, mid, p.second }));
				}
			}
		}
	}
}

std::string nfa2re(const NFA& nfa) {
	int cnt = nfa.get_vertex_count()+2;
	std::vector<std::unordered_map<int, std::string> > edges(cnt);
	edges[0][1] = std::string(1, EPSILON);
	for (const Trans& trans : nfa.get_transition()) {
		merge_edge(edges[trans.vertex_from + 1], trans.vertex_to + 1, std::string(1, trans.trans_symbol));
	}

	for (const int& state : nfa.get_final_states()) {
		edges[state + 1][cnt - 1] = std::string(1, EPSILON);
	}

	//remove states except the start and final
	remove_states(edges);

	return edges[0][cnt - 1];
}