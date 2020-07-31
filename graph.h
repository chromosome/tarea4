#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include "set.h"

using namespace std;

namespace dsa {

template <typename T>
class graph
{
	unordered_map<T, vector<T>> nodes;

	auto get_neighbours_set(vector<T> n) -> set<T> {
		set<T> N;
		copy(begin(n), end(n), inserter(N, begin(N)));
		return N;
	}

	auto bron_kerbosch(
			set<T> R, 
			set<T> P, 
			set<T> X, 
			vector<set<T>>& cliques
		) {
		if (P.empty() && X.empty() && (R.size() > 2)) {
			cliques.push_back(R);
			return;
		}

		set<T> aux(P);
		for (const auto& v: aux) {
			set<T> N = get_neighbours_set(nodes[v]);

			auto next_R = dsa::set_union(R, {v});
			auto next_P = dsa::set_intersection(P, N);
			auto next_X = dsa::set_intersection(X, N);

			bron_kerbosch(next_R, next_P, next_X, cliques);

			P = dsa::set_difference(P, {v});
			X = dsa::set_union(X, {v});
		}
	}

	auto clique_compact() -> void {

	}

public:
	auto add(T u1, T u2) -> void {
		auto& adj1 = nodes[u1];
		if (std::find(begin(adj1), end(adj1), u2) == end(adj1))
			adj1.push_back(u2);

		auto& adj2 = nodes[u2];
		if (std::find(begin(adj2), end(adj2), u1) == end(adj2))
			adj2.push_back(u1);
	}

	// DFS
	auto find(T u) -> bool {
		unordered_map<T, bool> visited;
		for (const auto& node: nodes)
			visited[node.first] = false;

		stack<T> to_visit;
		to_visit.push(begin(visited)->first);

		while(!to_visit.empty()) {
			auto n = to_visit.top();
			to_visit.pop();

			if (u == n)
				return true;

			if (visited[n] == false) {
				visited[n] = true;
				for (const auto& v: nodes[n])
					to_visit.push(v);
			}
		}

		return false;
	}

	auto follow(int n) -> vector<T> {
		auto compare = 
			[] (const pair<int, T>& a, const pair<int, T>& b) {
				if (a.first < b.first)
					return true;
				else if (a.first > b.first)
					return false;
				else
					if (a.second < b.second)
						return false;
					else
						return true;
			};
		priority_queue<pair<int, T>, 
					   vector<pair<int, T>>,
					   decltype(compare)> heap(compare);

		for (const auto& node: nodes)
			heap.push(make_pair(node.second.size(), node.first));

		if (n > nodes.size())
			n = nodes.size();

		vector<T> follow_count;
		for (int i = 0; i < n; ++i) {
			follow_count.push_back(heap.top().second);
			heap.pop();
		}

		return follow_count;
	}

	auto clique() -> vector<set<T>> {
		set<T> N;
		for (const auto& node: nodes)
			N.insert(node.first);

		vector<set<T>> cliques;
		bron_kerbosch(set<T>(), N, set<T>(), cliques);

		return cliques;
	}

	auto compact() -> graph<T> {
		cout << endl << __PRETTY_FUNCTION__ << endl;
		auto cliques = clique();

		vector<set<T>> inter;
		for (auto& clique: cliques) {
			for (auto& clique2: cliques) {
				if (clique != clique2) {
					auto i = dsa::set_intersection(clique, clique2);
					if (!i.empty() && (std::find(begin(inter), end(inter), clique2) == end(inter))) {
						inter.push_back(clique2);
					}
				}
			}
		}
		for (auto v: inter)
			cout << v << endl;


		cout << endl;
		return graph<T>();
	}

	// BFS
	auto print() -> void {

	}

	auto print_adjacency() -> void {
		for (const auto& node: nodes) {
			cout << node.first << ": [ ";
			for (const auto& v: node.second)
				cout << v << " ";
			cout << "]" << endl;
		}
		cout << endl;
	}
};

} // namespace dsa

#endif
