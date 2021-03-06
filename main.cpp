#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "graph.h"

using namespace std;

int main(int argc, char const *argv[])
{
	string filename;
	if (argc > 1)
		filename = argv[1];
	else {
		cout << "[Error] Debe ingresar un archivo de entrada" << endl;
		return -1;
	}

	dsa::graph<string> g;

	ifstream input(filename);
	string line;

	while (getline(input, line)) {
		stringstream line_stream(line);
		string field;

		if (line != "") {
			vector<string> fields;
			while (getline(line_stream, field, ' '))
				fields.push_back(field);

			if (fields[0] == "Add")
				g.add(fields[1], fields[2]);
			else if (fields[0] == "Find")
				cout << (g.find(fields[1]) ? "Yes": "No") << endl;
			else if (fields[0] == "Follow")
				for (const auto& n: g.follow(stoi(fields[1])))
					cout << n << endl;
			else if (fields[0] == "Clique")
				for (const auto& clique: g.clique())
					cout << clique << endl;
			else if (fields[0] == "Compact") {
				g.compact(); 
				g.print();
			}
		}
	}

	return 0;
}
