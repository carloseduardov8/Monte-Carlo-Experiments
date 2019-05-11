#include <iostream>			// For printing
#include <chrono>			// For time
#include <random>			// For RNG
#include <unordered_map>	// For storing grid structure
#include <string>			// For strings
#include <algorithm>		// To remove elements from vectors by value
#include <fstream>			// For file operations

// MACROS:
#define ptos(a) to_string(a.first) + "," + to_string(a.second)

using namespace std;

///////////////////////////
/// GLOBAL DECLARATIONS ///
///////////////////////////

// TYPEDEFS
typedef pair<int,int> node;									// Renames coordinate pair for convenience
// GLOBAL VARIABLES
bool verbose = false;										// True if program should print what it is doing
int pathSize = 5000;										// Size of path to generate
unordered_map<string,bool> hashTable; 						// Grid structure global variable
vector<node> path;											// Path being formed
mt19937 pivotGen, transformationGen;						// Mersenne Twister generators
uniform_int_distribution<int> uPivot, uTransformation;		// Uniform integer distributions
// FUNCTIONS
void initGenerator();										// Initiailzes the RNG
int samplePivot();											// Returns an int from 0 to pathSize-1
vector<int> sampleTransformation();							// Samples a matrix (represented as vector) with the transformation to be applied
void putInHash(int pivot);									// Puts all nodes of the walk into the hash table up to (and including) the node in position pivot
void generateRod();											// Generates an initial rod-shaped self-avoiding walk
void SAWtoFile(string filePath);							// Saves SAW coordinates to file

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
    os << "]";
    return os;
}



////////////////////	////////////////////	////////////////////
/// MAIN PROGRAM ///	/// MAIN PROGRAM ///	/// MAIN PROGRAM ///
////////////////////	////////////////////	////////////////////

int main(){

	// Initializes RNG classes:
	initGenerator();

	// Generates initial SAW:
	generateRod();

	// Clears hash table:
	hashTable.clear();
	// Chooses a pivot:
	int pivot = samplePivot();
	// Puts all elements before (and including) the pivot into the hash table:
	putInHash(pivot);
	// Samples a 2D transformation:
	vector<int> matrix = sampleTransformation();

	cout << "Pivot: " << pivot << endl;
	cout << "Transformation: " << matrix << endl;

}


/////////////////////////
/// UTILITY FUNCTIONS ///
/////////////////////////

// Puts all nodes of the walk into the hash table up to (and including) the node in position pivot:
void putInHash(int pivot){
	// Runs through the current SAW:
	for (int i=0; i<=pivot; i++){
		// Inserts node into hash table:
		hashTable.insert(make_pair(ptos(path[i]),true));
	}
}


// Generates an initial rod-shaped self-avoiding walk:
void generateRod(){
	// Loops through all points in the walk:
	for (int i=0; i<pathSize; i++){
		// Adds points (i,0) to the walk:
		path.push_back(node(i,0));
	}
}

// Saves SAW coordinates to file:
void SAWtoFile(string filePath){
	// Prepares output file:
	ofstream file;
	file.open(filePath);
	// Loops through SAW coordinates and save them:
	for (int i=0; i<path.size(); i++){
		file << path[i].first << " " << path[i].second << endl;
	}
}

// Initiailzes the RNG:
void initGenerator(){
	// Obtain a seed from the system clock:
	unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
	// Initializes MT generators:
	pivotGen = mt19937(seed1);
	transformationGen = mt19937(seed1/5);
	// Initializes pivot distribution:
	uPivot = uniform_int_distribution<int>(0.0, pathSize-1);
	// Initializes matrix transform distribution (rotation, reflexion, etc):
	uTransformation = uniform_int_distribution<int>(0.0, 6);
}

// Returns an int from 0 to pathSize-1:
int samplePivot(){
	return uPivot(pivotGen);
}

// Samples a matrix (represented as vector) with the transformation to be applied:
vector<int> sampleTransformation(){
	vector<int> matrix(4);
	// Samples from the distribution:
	int sample = uTransformation(transformationGen);
	// Checks which transform corresponds to int sample:
	switch(sample){
		// +90 rotation:
		case 0:
			matrix[0] = 0;  matrix[1] = -1;
			matrix[2] = 1;  matrix[3] = 0;
			break;
		// -90 rotation:
		case 1:
			matrix[0] = 0;  matrix[1] = 1;
			matrix[2] = -1; matrix[3] = 0;
			break;
		// 180 rotation:
		case 2:
			matrix[0] = -1; matrix[1] = 0;
			matrix[2] = 0;  matrix[3] = -1;
			break;
		// X-axis reflection:
		case 3:
			matrix[0] = 1;  matrix[1] = 0;
			matrix[2] = 0; 	matrix[3] = -1;
			break;
		// Y-axis reflection:
		case 4:
			matrix[0] = -1; matrix[1] = 0;
			matrix[2] = 0; 	matrix[3] = 1;
			break;
		// y=x reflection:
		case 5:
			matrix[0] = 0;  matrix[1] = 1;
			matrix[2] = 1;  matrix[3] = 0;
			break;
		// y=-x reflection:
		case 6:
			matrix[0] = 0;  matrix[1] = -1;
			matrix[2] = -1; matrix[3] = 0;
			break;
	}
	// Returns the sampled matrix:
	return matrix;

}
