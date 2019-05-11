#include <iostream>			// For printing
#include <chrono>			// For time
#include <random>			// For RNG
#include <unordered_map>	// For storing grid structure
#include <string>			// For strings
#include <algorithm>		// To remove elements from vectors by value

// MACROS:
#define ptos(a) to_string(a.first) + "," + to_string(a.second)

using namespace std;

///////////////////////////
/// GLOBAL DECLARATIONS ///
///////////////////////////

// TYPEDEFS
typedef pair<int,int> node;								// Renames coordinate pair for convenience
typedef struct tabu Tabu;
struct tabu {
	node avoid;
	node current;
	Tabu* pNext;
};
// GLOBAL VARIABLES
bool verbose = false;										// True if program should print what it is doing
int pathSize = 5000;										// Size of path to generate
unordered_map<string,bool> grid; 							// Grid structure global variable
vector<node> path;											// Path being formed
Tabu** tabuList;											// tabuList[i] is a linked list of nodes unvisitable from node path[i]
mt19937 gen;												// Mersenne Twister generator
uniform_real_distribution<double> uDis; 					// Uniform distribution
// FUNCTIONS
void walkTo(node a);										// Performs all operations to move the system to a node
node backtrack(int iteration);								// Walks backwards 1 node
bool visited(node a);										// Returns true if node was already visited in the grid
void availableNodes(int iteration, vector<node>* available);// Returns a number from 0 to 4 representing how many nodes the path may go to
node decideDestination(vector<node>* available);			// Returns the next node where the path should go to
void initGenerator();										// Initiailzes the RNG
double sampleUnif();										// Returns a real number from 0 to 1

// Helper function to print pairs:
template <typename T, typename S>
ostream& operator<<(ostream& os, const pair<T, S>& v)
{
    os << "(";
    os << v.first << ", "
       << v.second << ")";
    return os;
}

// Helper function to print vectors:
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    os << "[";
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]\n";
    return os;
}



////////////////////	////////////////////	////////////////////
/// MAIN PROGRAM ///	/// MAIN PROGRAM ///	/// MAIN PROGRAM ///
////////////////////	////////////////////	////////////////////

int main(){

	// Initializes RNG classes:
	initGenerator();
	// Initializes list of nodes that impede a self-avoiding path:
	tabuList = new Tabu*[pathSize]();

	// Initilizes grid:
	node current = node(0,0);
	walkTo(current);

	// Constructs a path:
	for (int i=0; i<pathSize-1; i++){
		if (i%100==0) cout << i << endl;
		// Retrieves available nodes:
		vector<node>* available = new vector<node>;
		availableNodes(i, available);
		// Checks if any node is available:
		if (available->size() != 0){
			// Decides where to go:
			current = decideDestination(available);
			walkTo(current);
			delete available;
			if (verbose) cout << "Moving to " << current << endl;
		// If no nodes are available:
		} else {
			// Undo time by 1 iteration:
			if (verbose) cout << "Deadend at node " << current << endl;
			current = backtrack(i);
			if (verbose) cout << "Backtracking to node " << current << endl;
			i-=2;
			// Impedes dead end from happening again:
			//delete tabu;
			//tabu =
		}
		delete available;
	}

	cout << "Generated path with length " << path.size() << endl;

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

node backtrack(int iteration){
	// Removes node from hash table:
	grid.erase(ptos(path.back()));
	// Adds this node to tabu list belonging to previous iteration. Runs through tabu list:
	Tabu** find = &(tabuList[iteration-1]);
	while (*find != NULL){
		find = &((*find)->pNext);
	}
	// Creates entry to be appended:
	Tabu* currentTabu = new Tabu;
	currentTabu->avoid = path.back();
	currentTabu->current = path.end()[-2];
	currentTabu->pNext = NULL;
	*find = currentTabu;
	// Removes last node from path:
	path.pop_back();
	// Clears this iteration's tabu list:
	tabuList[iteration] = NULL;
	// Returns previous node (new current position):
	return path.back();
}

// Returns true if node was already visited in the grid:
bool visited(node a){
	// Checks if node key exists in hash table that represents the grid:
	if (grid.count(ptos(a)) == 0){
		return false;
	} else {
		return true;
	}
}

// Returns a number from 0 to 4 representing how many nodes the path may go to:
void availableNodes(int iteration, vector<node>* available){
	// Retrieves current node:
	node a = path.back();
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
	// Retrieves tabu list for this iteration:
	Tabu* currentTabu = tabuList[iteration];
	if (verbose) cout << "Available before tabu removal: " << *available << endl;
	// Runs through tabu list for this iteration:
	while(currentTabu != NULL){
		// Checks if this tabu is relevant to this node:
		if (currentTabu->current == a){
			if (verbose) cout << "Found tabu " << currentTabu->avoid << endl;
			// Removes tabu from available vector:
			available->erase(remove(available->begin(), available->end(), currentTabu->avoid), available->end());
		}
		// Checks next tabu:
		currentTabu = currentTabu->pNext;
	}
	if (verbose) cout << "Available after tabu removal: " << *available << endl;
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
