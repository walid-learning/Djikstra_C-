#include <iostream>
#include "Parser.h"
#include <string>
#define NOTFOUND 0
#define SIZE 100

//#include "Grade.hpp"
using namespace std;
using namespace travel;

int main() {
	string file_stations = "data/s.csv";
	string file_connections = "data/c.csv";

	//string file_stations = "data/stations.csv";
	//string file_connections = "data/connections.csv";
	Parser R;
	int a; 
	uint64_t id_start, id_end; 
	char* name_start = new char[SIZE]; 
	char* name_end = new char[SIZE];

	R.read_stations(file_stations);
	R.read_connections(file_connections);
	vector<pair<uint64_t, uint64_t> > best_path;
	unordered_map<uint64_t, Station> stations_umap = R.get_stations_hashmap();
	unordered_map<uint64_t, unordered_map<uint64_t, uint64_t>> connections_umap = R.get_connections_hashmap();
	
	//vector<pair<uint64_t, uint64_t> > best_path = R.compute_and_display_travel(1166832, 1166840);
	//vector<pair<uint64_t, uint64_t> > best_path = R.compute_and_display_travel(1722, 2062);
	// 
	// -------------Bonus------------- //

	cout << " ################################## BIENVENUE DANS L APPLICATION RATP ##################################" << endl; 
	cout << endl; 
	cout << "Veuillez choisir une option SVP" << endl; 
	cout << endl; 
	cout << "1) Chemin avec ID des stations" << endl; 
	cout << "2) Chemin avec noms des stations" << endl; 

	cin >> a; 
	if (a == 2) {

		cout << "Donner le nom de la station de depart" << endl; 
		cin >> name_start; 
		cout << "Donner le nom de la station d arrivee" << endl; 
		cin >> name_end; 
		try {
			best_path = R.compute_and_display_travel(name_start, name_end);
		}

		catch (int i) {
			if (i == NOTFOUND)
				cerr << "Fail !" << endl;
			
		}
	}

	else {
		cout << "Donner le ID de la station de depart" << endl;
		cin >> id_start;
		cout << "Donner le ID de la station d arrivee" << endl;
		cin >> id_end;
		try {
			best_path = R.compute_and_display_travel(id_start, id_end);
		}

		catch (int i) {
			if (i == NOTFOUND)
				cerr << "Fail !" << endl;


		}
	}
	
	//---------------------------------//
	
	vector < pair<uint64_t, uint64_t>>::iterator p;
	
}