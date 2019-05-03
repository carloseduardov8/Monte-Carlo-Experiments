#include <iostream>			// For printing
#include <chrono>			// For time
#include <random>			// For RNG
#include <unordered_map>	// For storing grid structure
#include <string>			// For strings

// MACROS:
#define ptos(a) to_string(a.first) + "," + to_string(a.second)

using namespace std;

///////////////////////////
/// GLOBAL DECLARATIONS ///
///////////////////////////

// TYPEDEFS
typedef pair<int,int> node;								// Renames coordinate pair for convenience
// GLOBAL VARIABLES
unordered_map<string,bool> grid; 						// Grid structure global variable
vector<node> path;										// Path being formed
mt19937 gen;											// Mersenne Twister generator
uniform_real_distribution<double> uDis; 				// Uniform distribution
// FUNCTIONS
void walkTo(node a);									// Performs all operations to move the system to a node
node backtrack();										// Walks backwards 1 node
bool visited(node a);									// Returns true if node was already visited in the grid
void availableNodes(node a, vector<node>* available);	// Returns a number from 0 to 4 representing how many nodes the path may go to
node decideDestination(vector<node>* available);		// Returns the next node where the path should go to
void initGenerator();									// Initiailzes the RNG
double sampleUnif();									// Returns a real number from 0 to 1

// Helper function to print pairs:
template <typename T, typename S>
ostream& operator<<(ostream& os, const pair<T, S>& v)
{
    os << "(";
    os << v.first << ", "
       << v.second << ")";
    return os;
}


////////////////////	////////////////////	////////////////////
/// MAIN PROGRAM ///	/// MAIN PROGRAM ///	/// MAIN PROGRAM ///
////////////////////	////////////////////	////////////////////

int main(){

	// Initializes RNG classes:
	initGenerator();

	// Initilizes grid:
	node current = node(0,0);
	walkTo(current);

	for (int i=0; i<1000; i++){
		// Retrieves available nodes:
		vector<node>* available = new vector<node>;
		availableNodes(current, available);
		// Checks if any node is available:
		if (available->size() != 0){
			// Decides where to go:
			current = decideDestination(available);
			walkTo(current);
			delete available;
			cout << current << endl;
		// If no nodes are available:
		} else {
			// Undo time by 1 iteration:
			 current = backtrack();
		}
	}

}


/////////////////////////
/// UTILITY FUNCTIONS ///
/////////////////////////


// Performs all operations to move the system to a node:
void walkTo(node a){
	// Inserts node into hash table:
	grid.insert(make_pair(ptos(a),true));
	// Inserts node into path:
	path.push_back(a);
}

node backtrack(){
	// Removes node from hash table:
	grid.erase(ptos(path.back()));
	// Removes last node from path:
	path.pop_back();
	// Returns previous node (new current position):
	return path.back();
}

// Returns true if node was already visited in the grid:
bool visited(node a){
	if (grid.count(ptos(a)) == 0){
		return false;
	} else {
		return true;
	}
}

// Returns a number from 0 to 4 representing how many nodes the path may go to:
void availableNodes(node a, vector<node>* available){
	// Checks each grid possibility individually:
	if (grid.count(ptos(node(a.first,a.second+1))) == 0){
		available->push_back(node(a.first,a.second+1));
	}
	if (grid.count(ptos(node(a.first,a.second-1))) == 0){
		available->push_back(node(a.first,a.second-1));
	}
	if (grid.count(ptos(node(a.first+1,a.second))) == 0){
		available->push_back(node(a.first+1,a.second));
	}
	if (grid.count(ptos(node(a.first-1,a.second))) == 0){
		available->push_back(node(a.first-1,a.second));
	}
}

// Returns the next node where the path should go to:
node decideDestination(vector<node>* available){
	// Calculates probabilities:
	double unif = sampleUnif();
	double prob = 1.0/available->size();
	// Decides where to go:
	for (int i = 1; i<=4; i++){
		// Checks if node was selected:
		if (unif <= prob*i){
			// Elects the node:
			return available->at(i-1);
		}
	}
	// If loop finished, prints out an error:
	cout << endl << "ERROR: Couldn't decide on a node to continue to. Prob = " << prob << " and available nodes = " << available->size() << endl;
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
double sampleUnif(){
	return uDis(gen);
}
