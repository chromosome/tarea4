#ifndef SET_H
#define SET_H

#include <iostream>
#include <algorithm>
#include <set>

namespace dsa {
	template<typename T>
	auto set_union (std::set<T> A, std::set<T> B) -> std::set<T> {
		std::set<T> C;
		std::set_union(std::begin(A), std::end(A),
					   std::begin(B), std::end(B),
					   inserter(C, std::begin(C)));
		return C;
	}

	template<typename T>
	auto set_intersection (std::set<T> A, std::set<T> B) -> std::set<T> {
		std::set<T> C;
		std::set_intersection(std::begin(A), std::end(A),
							  std::begin(B), std::end(B),
							  inserter(C, std::begin(C)));
		return C;
	}

	template<typename T>
	auto set_difference (std::set<T> A, std::set<T> B) -> std::set<T> {
		std::set<T> C;
		std::set_difference(std::begin(A), std::end(A),
							std::begin(B), std::end(B),
							inserter(C, std::begin(C)));
		return C;
	}
}

#endif
