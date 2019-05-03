#include <iostream>			// For printing
#include <chrono>			// For time
#include <random>			// For RNG
#include <unordered_map>	// For storing grid structure
#include <string>			// For strings

using namespace std;

///////////////////////////
/// GLOBAL DECLARATIONS ///
///////////////////////////

// TYPEDEFS
typedef pair<int,int> node;				// Renames coordinate pair for convenience
// GLOBAL VARIABLES
unordered_map<string,bool> grid; 		// Grid structure global variable
mt19937 gen;							// Mersenne Twister generator
uniform_real_distribution<double> uDis; // Uniform distribution
// FUNCTIONS
string ptos(pair<int,int> a); 			// Converts a pair to a string
void walkTo(node a);					// Performs all operations to move the system to a node
bool visited(node a);					// Returns true if node was already visited in the grid
void initGenerator();					// Initiailzes the RNG
double unif();							// Returns a real number from 0 to 1


////////////////////
/// MAIN PROGRAM ///
////////////////////

int main(){

	// Initializes RNG classes:
	initGenerator();

	// Grid structure:
	node init = node(0,0);
	node init2 = node(0,1);
	node init3 = node(0,2);
	walkTo(init);

	cout << visited(init) << " " << unif() << endl;

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

// Initiailzes the RNG:
void initGenerator(){
	// Obtain a seed from the system clock:
	unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
	// Initializes MT generator:
	gen = mt19937(seed1);
	// Initializes normal distribution:
	uDis = uniform_real_distribution<double>(0.0, 1.0);
}

// Returns a real number from 0 to 1:
double unif(){
	return uDis(gen);
}
