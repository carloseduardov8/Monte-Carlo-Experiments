#include <iostream>			// For printing
#include <chrono>			// For time
#include <random>			// For RNG
#include <unordered_map>	// For storing grid structure
#include <string>			// For strings

using namespace std;

///////////////////////////
/// GLOBAL DECLARATIONS ///
///////////////////////////

// Grid structure global variable:
unordered_map<string,bool> grid;
// Renames coordinate pair for convenience:
typedef pair<int,int> node;
// Converts a pair to a string:
string ptos(pair<int,int> a);
// Performs all operations to move the system to a node:
void walkTo(node a);
// Returns true if node was already visited in the grid:
bool visited(node a);


////////////////////
/// MAIN PROGRAM ///
////////////////////

int main(){

	// Obtain a seed from the system clock:
	unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
	// Initializes MT generator:
	mt19937 gen = mt19937(seed1);
	// Initializes normal distribution:
	uniform_real_distribution<double> uDis(0.0, 1.0);

	// Grid structure:
	node init = node(0,0);
	node init2 = node(0,1);
	node init3 = node(0,2);
	walkTo(init);

	//uDis(gen)

	cout << visited(init) << endl;

}


/////////////////////////
/// UTILITY FUNCTIONS ///
/////////////////////////

// Converts a pair to a string:
string ptos(pair<int,int> a){
	return to_string(a.first) + "," + to_string(a.second);
}

// Performs all operations to move the system to a node:
void walkTo(node a){
	// Inserts node into hash table:
	grid.insert(make_pair(ptos(a),true));
}

// Returns true if node was already visited in the grid:
bool visited(node a){
	if (grid.count(ptos(a)) == 0){
		return false;
	} else {
		return true;
	}
}
