#include<iostream>
#include<fstream>
#include"Parser.h"



#define NOTFOUND 0
#define INF 50000

using namespace std;
using namespace travel;

void Parser::read_stations(const std::string& _fileName)  {
	ifstream monFlux(_fileName);

	if (monFlux)
	{
		string Ligne;
		string info[5];
		string Id;
		int i = 0;
		getline(monFlux, Ligne);
		while (getline(monFlux, Ligne, ',')) {

			info[i] = Ligne;
			i++;
			if (i == 4) {  //pour le dernier element de la Ligne
				getline(monFlux, Ligne, '\n');
				i++;
			}
			if (i == 5) {
				Station s;
				Id = info[1];
				s.Name = info[0];
				s.Line_id = info[2];
				s.Address = info[3];
				s.Line_Name = info[4];
				stations_hashmap[stoi(Id)] = s;
				NametoId_hashmap[info[0]] = stoi(info[1]);
				i = 0;
				
			}
		}		
	}
	else{
		cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
	}


};


void Parser::read_connections(const std::string& _fileName) {
	ifstream monFlux(_fileName);

	if (monFlux)
	{
		string Ligne;
		string info[3];
		
		int i = 0;
		getline(monFlux, Ligne);
		while (getline(monFlux, Ligne, ',')) {
			info[i] = Ligne;
			i++;
			if (i == 2) {
				getline(monFlux, Ligne, '\n');
				info[i] = Ligne;
				connections_hashmap[stoi(info[0])][stoi(info[1])] = stoi(info[2]);
				i = 0;
			}
			
		}
	}
}

// compute travel en utilisant le nom des stations 
vector<pair<uint64_t, uint64_t> > Parser::compute_travel(const string& start, const string& end) {
	uint64_t s_found = 0;
	uint64_t e_found = 0;

	// Gerer les exceptions 
	for (auto station_iterator = NametoId_hashmap.begin(); station_iterator != NametoId_hashmap.end(); station_iterator++) {
		if (station_iterator->first == start)
			s_found++;
		if (station_iterator->first == end)
			e_found++;

	}
	if (s_found == 0 || e_found == 0) throw NOTFOUND;


	uint64_t _start = NametoId_hashmap[start];
	uint64_t _end = NametoId_hashmap[end];
	unordered_map<uint64_t, Station>graph(stations_hashmap);// (node_Id,distance)
	unordered_map<uint64_t, uint64_t>pred;
	unordered_map<uint64_t, uint64_t>distance;

	uint64_t Id_node_min_distance = _start;
	uint64_t distance_min;


	////
	// initialisation de graph (node_Id, node_distance)
	for (auto station_iterator = stations_hashmap.begin(); station_iterator != stations_hashmap.end(); station_iterator++) {
		distance[station_iterator->first] = INF;
	}
	distance[Id_node_min_distance] = 0;
	////


	while (!graph.empty() && Id_node_min_distance != _end) {
		///// 
		// recherche du noeud avec distance minimal
		distance_min = INF;
		for (auto iterator = graph.begin(); iterator != graph.end(); iterator++) {

			if (distance[iterator->first] < distance_min) {
				distance_min = distance[iterator->first];
				Id_node_min_distance = iterator->first;
			}
		}

		graph.erase(Id_node_min_distance);
		////

		////
		// Mise a jour des distances de voisins
		for (auto iterator = connections_hashmap[Id_node_min_distance].begin(); iterator != connections_hashmap[Id_node_min_distance].end(); iterator++) {

			if (distance_min + iterator->second < distance[iterator->first]) {

				distance[iterator->first] = distance_min + iterator->second;
				pred[iterator->first] = Id_node_min_distance;
			}
		}
		////
	}


	////
	// engregistrement du meilleur chemin dans un vecteur
	uint64_t node = _end;
	vector<std::pair<uint64_t, uint64_t>> Path;
	while (node != _start) {

		Path.push_back(make_pair(node, connections_hashmap[pred[node]][node]));
		node = pred[node];
	}
	//
	return Path;

}

// compute travel en utilisant les id
vector<pair<uint64_t, uint64_t> > Parser::compute_travel(uint64_t _start, uint64_t _end) {

	unordered_map<uint64_t, Station>graph(stations_hashmap);// (node_Id,distance)
	unordered_map<uint64_t, uint64_t>pred;
	unordered_map<uint64_t, uint64_t>distance;

	uint64_t Id_node_min_distance = _start;
	uint64_t distance_min;

	////
	// initialisation de graph (node_Id, node_distance)
	for (auto station_iterator = stations_hashmap.begin(); station_iterator != stations_hashmap.end(); station_iterator++) {
		distance[station_iterator->first] = INF;
	}
	distance[Id_node_min_distance] = 0;
	////
	

	while (!graph.empty() && Id_node_min_distance != _end) {
		///// 
		// recherche du noeud avec distance minimal
		distance_min = INF;
		for (auto iterator = graph.begin(); iterator != graph.end(); iterator++) {
			
			if (distance[iterator->first] < distance_min) {
				distance_min = distance[iterator->first];
				Id_node_min_distance = iterator->first;
			}
		} 
		
		graph.erase(Id_node_min_distance);
		////
		
		////
		// Mise a jour des distances de voisins
		for (auto iterator = connections_hashmap[Id_node_min_distance].begin(); iterator != connections_hashmap[Id_node_min_distance].end(); iterator++) {
			
			if (distance_min + iterator->second < distance[iterator->first]) {
				
				distance[iterator->first] = distance_min + iterator->second;
				pred[iterator->first] = Id_node_min_distance;
			}
		}
		////
	}
	

	////
	// engregistrement du meilleur chemin dans un vecteur
	uint64_t node = _end;
	vector<std::pair<uint64_t, uint64_t>> Path;
	while (node != _start) {
		
		Path.push_back(make_pair(node,connections_hashmap[pred[node]][node] ));
		node = pred[node];
	}	
	//
	return Path;
}

vector<std::pair<uint64_t, uint64_t>> Parser::compute_and_display_travel(uint64_t _start, uint64_t _end) {
	vector<std::pair<uint64_t, uint64_t>> Path = compute_travel(_start, _end);
	cout << "\nBest way from " << stations_hashmap[_start].Name << "(line " << stations_hashmap[_start].Line_id << " ) to: " << stations_hashmap[_end].Name << "(line " << stations_hashmap[_end].Line_id << ") is : " << endl;
	uint64_t time = 0;
	// parcours inverse 
	for (auto it = Path.rbegin(); it != Path.rend(); ++it)
	{	
		
		time += it->second;
		cout << stations_hashmap[it->first] << "  ---" << time/60 << " minutes and "<< time - uint64_t(time / 60) * 60 <<" secs --- "<< endl;
		
	}
	
	cout << "\nAfter " << time / 60 << " minutes and " << time - uint64_t(time / 60) * 60 << " seconds, you have reached your destination!" << std::endl;

	return Path;
}


vector<std::pair<uint64_t, uint64_t>> Parser::compute_and_display_travel(const string& start, const string&end) {
	vector<std::pair<uint64_t, uint64_t>> Path;

	try {
		Path = compute_travel(start, end);
	}
	catch (int j)
	{	if (j == NOTFOUND)
		cerr << "Station non trouvee \nVeuillez verifier votre saisie " << endl;
		throw;
	}

	uint64_t _start = NametoId_hashmap[start];
	uint64_t _end = NametoId_hashmap[end];
	cout << "\nBest way from " << start << "(Line " << stations_hashmap[_start].Line_id << ") to: " << end << "(Line " << stations_hashmap[_end].Line_id << ") is : " << endl;
	uint64_t time = 0;
	// parcours inverse 
	for (auto it = Path.rbegin(); it != Path.rend(); ++it)
	{

		time += it->second;
		cout << stations_hashmap[it->first] << "  ---" << time / 60 << " minutes and " << time - uint64_t(time / 60) * 60 << " secs --- " << endl;

	}

	cout << "\nAfter " << time / 60 << " minutes and " << time - uint64_t(time / 60) * 60 << " seconds, you have reached your destination!" << std::endl;

	return Path;
}
