#ifndef SET_H
#define SET_H

#include <iostream>
#include <algorithm>
#include <set>


/*  Funcion de utilidad para imprimir set<T> en pantalla.
 */
template<typename T>
std::ostream& operator << (std::ostream& out, const std::set<T> S) {
	for (const auto& s: S)
		out << s << " ";
	return out;
}

namespace dsa {

	/*  Funcion de utilidad para simplificar el calculo de la union de dos 
	 *  conjuntos.
	 */
	template <typename T>
	auto set_union (std::set<T> A, std::set<T> B) -> std::set<T> {
		std::set<T> C;
		std::set_union(std::begin(A), std::end(A),
					   std::begin(B), std::end(B),
					   inserter(C, std::begin(C)));
		return C;
	}

	/*  Funcion de utilidad para simplificar el calculo de la interseccion de
	 *  dos conjuntos.
	 */
	template <typename T>
	auto set_intersection (std::set<T> A, std::set<T> B) -> std::set<T> {
		std::set<T> C;
		std::set_intersection(std::begin(A), std::end(A),
							  std::begin(B), std::end(B),
							  inserter(C, std::begin(C)));
		return C;
	}

	/*  Funcion de utilidad para simplificar el calculo de la diferencia entre
	 *  dos conjuntos.
	 */
	template <typename T>
	auto set_difference (std::set<T> A, std::set<T> B) -> std::set<T> {
		std::set<T> C;
		std::set_difference(std::begin(A), std::end(A),
							std::begin(B), std::end(B),
							inserter(C, std::begin(C)));
		return C;
	}

} // namspace dsa

#endif
