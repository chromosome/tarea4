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
	/*  Mapa de nodos. Esta implementacion permite mantener el acceso a las
	 *  listas de adyacencia en tiempo O(1), sacrificando espacio ya que los
	 *  'unordered_map' estan implementados sobre una tabla hash.
	 */
	unordered_map<T, vector<T>> nodes;

	/*  Funcion de utilidad para obtener el conjunto de nodos del grafo, desde 
	 *  el mapa de nodos.
	 */
	auto get_nodes_set() -> set<T> {
		set<T> V;
		transform(begin(nodes), end(nodes), inserter(V, begin(V)),
			[](const pair<T, vector<T>>& p){ return p.first; });
		return V;
	}

	/*  Funcion de utilidad para obtener el conjunto nodos adyacentes de la 
	 *  lista de adyacencia de un nodo 'u' perteneciente al grafo.
	 */
	auto get_neighbours_set(T u) -> set<T> {
		set<T> N;
		copy(begin(nodes[u]), end(nodes[u]), inserter(N, begin(N)));
		return N;
	}

	/*  Implementacion del algoritmo de Bron-Kerbosch para obtener todos los 
	 *  cliques maximales mayores a 2 elementos. Partiendo desde un nodo 'v'
	 *  cualquiera, se toma cada nodo de su lista de adyacencia y se agregan a
	 *  R uno a la vez. Luego se agrega el resto de los nodos recursivamente 
	 *  desde la intersección de P y los nodos adyacentes a 'v', donde P son 
	 *  los nodos que potencialmente pertenecen a un clique y X son los nodos 
	 *  que ya fueron considerados por lo que no deben agregarse a R. Cuando no
	 *  quedan elementos en P y en X se ha encontrado un clique maximal. Luego 
	 *  este es agregado a un arreglo de cliques que es pasado por referencia 
	 *  para poder utilizarlos fuera de la funcion.
	 */
	int count = 0;
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
			set<T> N = get_neighbours_set(v);

			auto next_R = dsa::set_union(R, {v});
			auto next_P = dsa::set_intersection(P, N);
			auto next_X = dsa::set_intersection(X, N);

			bron_kerbosch(next_R, next_P, next_X, cliques);

			P = dsa::set_difference(P, {v});
			X = dsa::set_union(X, {v});
		}
	}

	/*  Funcion de utilidad para eliminar nodos del grafo. Recorre todas las 
	 *  listas de adyacencia donde se encuentre el nodo 'u' eliminandolo. 
	 *  Finalmente elimina 'u' del mapa de nodos.
	 */
	auto remove_node(T u) -> void {
		auto u_it = nodes.find(u);
		if (u_it != end(nodes)) {
			for (const auto& v: nodes[u]) {
				auto& adj = nodes[v];
				auto it = std::find(begin(adj), end(adj), u);
				if (it != end(adj))
					adj.erase(it);
			}
			nodes.erase(u_it);
		}
	}

	/*  Funcion de utilidad para obtener todos los nodos adyacentes a un clique
	 *  en particular. Obtiene la union de todos los conjuntos de adyacencia 
	 *  para cada nodo en el clique. Luego obtiene el conjunto de diferencia
	 *  entre el conjunto de union y el clique.
	 */
	auto get_clique_neighours_set(set<T> clique) -> set<T> {
		set<T> neighbours;
		for (const auto& node: clique)
			neighbours = dsa::set_union(neighbours, get_neighbours_set(node));

		return dsa::set_difference(neighbours, clique);
	}

	/*  Funcion de utilidad para eliminar un clique particular del grafo. Esta
	 *  llama a la funcion 'remove_node' para cada nodo del clique.
	 */
	auto remove_clique(set<T> clique) -> void {
		for (const auto& u: clique)
			remove_node(u);
	}

	/*  Funcion de utilidad para compactar un clique del grafo en un nodo con
	 *  con llave 'key'. Primero obtiene el conjunto de nodos ayacentes al 
	 *  clique a eliminar y luego elimina todos los nodos del clique mediante
	 *  la funcion 'remove_clique'. Finalmente utiliza la funcion 'add' para
	 *  insertar un nuevo nodo con llave 'key' y crear una arista para cada
	 *  nodo en el conjunto de adyacencia.
	 */
	auto compact_clique(set<T> clique, T key) -> void {
		auto neighbours = get_clique_neighours_set(clique);
		remove_clique(clique);
		for (const auto& n: neighbours)
			add(n, key);
	}

public:

	/*  Funcion "add" implementa la relacion de dos usuarios. Se aprovecha el
	 *  operator [] de "unordered_map" para insertar un nodo si no se encuentra
	 *  en el mapa de nodos. Esto crea una lista de adyacencia vacia para el  
	 *  nodo agregado. En caso de que el nodo ya esté en el mapa de nodos, se 
	 *  busca el segundo nodo en su lista de adyacencia a través de una 
	 *  referencia y si no se encuentra en esta se agrega.
	 */
	auto add(T u1, T u2) -> void {
		auto& adj1 = nodes[u1];
		if (std::find(begin(adj1), end(adj1), u2) == end(adj1))
			adj1.push_back(u2);

		auto& adj2 = nodes[u2];
		if (std::find(begin(adj2), end(adj2), u1) == end(adj2))
			adj2.push_back(u1);
	}

	/*  La funcion "find" implementa la busqueda de un nodo 'u' mediante un 
	 *  algoritmo iterativo de DFS.
	 */
	auto find(T u) -> bool {
		unordered_map<T, bool> visited;
		for (const auto& node: nodes)
			visited[node.first] = false;

		stack<T> to_visit;
		to_visit.push(begin(visited)->first);

		while (!to_visit.empty()) {
			auto n = to_visit.top();
			to_visit.pop();

			if (u == n)
				return true;

			if (!visited[n]) {
				visited[n] = true;
				for (const auto& v: nodes[n])
					to_visit.push(v);
			}
		}

		return false;
	}

	/*  La funcion print imprime en pantalla las aristas del grafo mediante un 
	 *  recorrido implementado por un algoritmo de BFS.
	 */
	auto print() -> void {
		unordered_map<T, bool> visited;
		for (const auto& node: nodes)
			visited[node.first] = false;

		queue<T> to_visit;
		to_visit.push(begin(visited)->first);

		while (!to_visit.empty()) {
			auto n = to_visit.front();
			to_visit.pop();

			if (!visited[n]) {
				visited[n] = true;
				for (const auto& v: nodes[n]) {
					to_visit.push(v);
					if (!visited[v])
						cout << "(" << n << ", " << v << ")" << endl;
				}
			}
		}
	}

	/*  La funcion "follow" retorna un arreglo con los n nodos de mayor grado. 
	 *  Para cada nodo en el mapa de nodos se obtiene el tamaño de su lista de 
	 *  adyacencia, lo que representa el grado del nodo. Luego se agregan a una
	 *  cola de prioridad que utiliza una funcion de comparacion que los ordena
	 *  de forma descendente. En caso de tener dos o mas nodos con el mismo 
	 *  grado se ordenan de forma lexicografica.
	 */
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

	/*  La funcion "clique" retorna un arreglo con todos los cliques de tamaño
	 *  mayor a 2 mediante el uso del algoritmo de Bron-Kerbosch, que enumera
	 *  los cliques de forma recursiva.
	 */
	auto clique() -> vector<set<T>> {
		auto N = get_nodes_set();

		vector<set<T>> cliques;
		bron_kerbosch(set<T>(), N, set<T>(), cliques);

		return cliques;
	}

	/*  La funcion "compact" compacta todos los cliques del grafo en nodos.
	 *  Para manejar los casos en que los cliques comparten nodos se ordenan en
	 *  orden de tamaño descendente, se saca el mayor del arreglo y se buscan 
	 *  los conjuntos de interseccion con el resto de los cliques. Luego se 
	 *  restan los nodos comunes de los cliques menores y se compacta el clique
	 *  mayor. Este proceso se continua hasta que no quedan cliques en el 
	 *  arreglo. Si al restar nodos del clique, este queda con 2 o menos nodos
	 *  no se compacta.
	 */
	auto compact() -> void {
		auto cliques = clique();
		auto compare = 
			[] (const set<T>& A, const set<T>& B) {
				return A.size() < B.size() ? true : false;
			};
		sort(begin(cliques), end(cliques), compare);

		string compact_key = "Componente";
		size_t compact_count = 1;
		while (!cliques.empty()) {
			auto c = cliques.back();
			cliques.pop_back();

			if (c.size() > 2) {
				for (auto& other: cliques) {
					auto inter = dsa::set_intersection(c, other);
					if (!inter.empty())
						other = dsa::set_difference(other, inter);
				}
				sort(begin(cliques), end(cliques), compare);

				compact_clique(c, compact_key+" "+to_string(compact_count));
				++compact_count;
			}
		}
	}

	/*  Funcion de utilidad para imprimir las listas de adyacencia en pantalla.
	 */
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
