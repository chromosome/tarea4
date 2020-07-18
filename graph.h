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
// template<typename T>
// using u_set = unordered_set<T>;
template<typename K, typename V>
using u_map = unordered_map<K, V>;

namespace dsa {

template <typename T>
class graph
{
	u_map<T, vector<T>> nodes;

	auto bron_kerbosch__ (set<T> R, set<T> P, set<T> X) {
		if (P.empty() && X.empty() && (R.size() > 2)) {
			cout << "clique: "<< R << endl;
			return;
		}

		set<T> aux(P);
		for (const auto& v: aux) {
			// vecinos de v
			set<T> N;
			for (const auto& n: nodes[v])
				N.insert(n);

			auto next_R(R);
			next_R.insert(v);

			set<T> next_P;
			set_intersection(begin(P), end(P), 
							 begin(N), end(N),
							 inserter(next_P, begin(P)));

			set<T> next_X;
			set_intersection(begin(X), end(X),
							 begin(N), end(N),
							 inserter(next_X, begin(X)));

			// cout << endl;
			// cout << "N: " << N << endl;
			// cout << "next_R: " << next_R << endl;
			// cout << "next_P: " << next_P << endl;
			// cout << "next_X: " << next_X << endl;
			// cin.get();
			bron_kerbosch(next_R, next_P, next_X);

			P.erase(P.find(v));
			X.insert(v);
		}

	}

	auto bron_kerbosch (set<T> R, set<T> P, set<T> X) {
		if (P.empty() && X.empty() && (R.size() > 2)) {
			cout << "clique: "<< R << endl;
			return;
		}

		set<T> aux(P);
		for (const auto& v: aux) {
			set<T> N;
			for (const auto& n: nodes[v])
				N.insert(n);

			auto next_R = dsa::set_union(R, {v});
			auto next_P = dsa::set_intersection(P, N);
			auto next_X = dsa::set_intersection(X, N);

			bron_kerbosch(next_R, next_P, next_X);

			P = dsa::set_difference(P, {v});
			X = dsa::set_union(X, {v});
		}
	}

	auto bron_kerbosch_pivot (set<T> R, set<T> P, set<T> X) {
		if (P.empty() && X.empty() && (R.size() > 2)) {
			cout << "clique: "<< R << endl;
			return;
		}

		set<T> aux(P);
		for (const auto& v: aux) {
			set<T> N;
			for (const auto& n: nodes[v])
				N.insert(n);

			auto next_R = dsa::set_union(R, {v});
			auto next_P = dsa::set_intersection(P, N);
			auto next_X = dsa::set_intersection(X, N);

			bron_kerbosch(next_R, next_P, next_X);

			P = dsa::set_difference(P, {v});
			X = dsa::set_union(X, {v});
		}
	}

public:
	auto add (T u1, T u2) -> void {
		auto& adj1 = nodes[u1];
		if (std::find(begin(adj1), end(adj1), u2) == end(adj1))
			adj1.push_back(u2);

		auto& adj2 = nodes[u2];
		if (std::find(begin(adj2), end(adj2), u1) == end(adj2))
			adj2.push_back(u1);
	}

	// DFS
	auto find (T u) -> bool {
		u_map<T, bool> visited;
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

	auto follow (int n) -> vector<T> {
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
		// priority_queue<pair<int, T>> heap;
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

	auto clique () /* -> vector<graph<T>> */ {
		cout << "clique" << endl;
		set<T> N;
		for (const auto& node: nodes)
			N.insert(node.first);
		bron_kerbosch(set<T>(), N, set<T>());
		// return vector<graph<T>>();
	}

	auto compact () -> graph<T> {
		cout << "compact" << endl;
		return graph<T>();
	}

	auto print_adjacency() {
		for (const auto& node: nodes) {
			cout << node.first << ": [ ";
			for (const auto& v: node.second)
				cout << v << " ";
			cout << "]" << endl;
		}
		cout << endl;
	}

	auto size () -> size_t {
		return nodes.size();
	}
};


template<typename T>
ostream& operator << (ostream& out, const set<T> S) {
	for (const auto& s: S)
		out << s << " ";
	return out;
}


} // namespace dsa

#endif
