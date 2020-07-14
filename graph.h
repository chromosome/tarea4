#ifndef GRAPH_H
#define GRAPH_H

// namespace graph {

#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <list>
#include <stack>
#include <queue>
#include <unordered_map>

using namespace std;

template <typename T>
class graph
{
	unordered_map<T, list<T>> nodes;

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

	auto clique () -> graph<T> {
		cout << "clique" << endl;
		return graph<T>();
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

// } // namespace graph

#endif
