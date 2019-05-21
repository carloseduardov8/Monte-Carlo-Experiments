#include <iostream>			// For printing
#include <chrono>			// For time
#include <random>			// For RNG
#include <unordered_map>	// For storing grid structure
#include <string>			// For strings
#include <algorithm>		// To remove elements from vectors by value
#include <fstream>			// For file operations
#include <cmath>			// For mathematical operations

// MACROS:
#define ptos(a) to_string(a.first) + "," + to_string(a.second)
#define N 10 // SAW is counted by steps, so pathSize = N + 1

using namespace std;

///////////////////////////
/// GLOBAL DECLARATIONS ///
///////////////////////////

// TYPEDEFS
typedef pair<int,int> node;									// Renames coordinate pair for convenience
// GLOBAL VARIABLES
bool verbose = false;										// True if program should print what it is doing
int pathSize = N + 1;										// Size (in nodes) of path to generate. SAWs are usually counted by steps (N)
int totalSamples = 1010000;									// Total number of samples to collect (excluding thermalization)
int thermalization = 10000;									// Number of samples to discard before collecting statistics
unordered_map<string,bool> hashTable; 						// Grid structure global variable
vector<node> path;											// Path being formed
mt19937 pivotGen, transformationGen;						// Mersenne Twister generators
uniform_int_distribution<int> uPivot, uTransformation;		// Uniform integer distributions
// FUNCTIONS
void initGenerator();										// Initiailzes the RNG
int samplePivot();											// Returns an int from 0 to pathSize-1
vector<int> sampleTransformation();							// Samples a matrix (represented as vector) with the transformation to be applied
void putInHash(int pivot);									// Puts all nodes of the walk into the hash table up to (and including) the node in position pivot
bool checkCollision(int pivot, vector<int>* matrix);		// Checks if [pivot+1, n-1] elements, under transfomation matrix, collide with [0, pivot] elements. If not, copies them to the current path
double endToEndDistance();									// Returns the end to end squared distance of the walk
long long int numOfHorseshoes();							// Returns the number of horseshoes contained in the walk
void generateRod();											// Generates an initial rod-shaped self-avoiding walk
void SAWtoFile(string filePath);							// Saves SAW coordinates to file
template <class T>
void _enumerate(T* result, T (*f)(), int it=1);				// Enumerates all SAWs with N steps, executes function f at each SAW and saves the sum to result
void enumerateWrapper();									// Initializes enumeration scenario

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

long long int countMe(){
	return 1;
}


////////////////////	////////////////////	////////////////////
/// MAIN PROGRAM ///	/// MAIN PROGRAM ///	/// MAIN PROGRAM ///
////////////////////	////////////////////	////////////////////

int main(){

	// Initializes RNG classes:
	initGenerator();

	// Generates initial SAW:
	generateRod();

	int samplesCount = 0;
	int stepCount = 0;
	double accumulator = 0;

	while(samplesCount < totalSamples){
		// Clears hash table:
		hashTable.clear();
		// Chooses a pivot:
		int pivot = samplePivot();
		// Puts all elements before (and including) the pivot into the hash table:
		putInHash(pivot);
		// Samples a 2D transformation:
		vector<int> matrix = sampleTransformation();
		// Checks if transformation, when applied to the [k+1,n-1] path elements, collides with [0,k]:
		checkCollision(pivot, &matrix);
		// Increments number of samples:
		samplesCount++;
		if (samplesCount > thermalization){
			// Selects last node in the walk:
			node endNode = path[pathSize-1];
			// Adds end-to-end square distance to the MC accumulator:
			accumulator += endToEndDistance();
		}
	}

	// Calculates mean of accumulator:
	double result = accumulator*1.0/(totalSamples-thermalization);

	cout << "Result: " << result << endl;

}


/////////////////////////
/// UTILITY FUNCTIONS ///
/////////////////////////

// Returns the number of horseshoes contained in the walk:
long long int numOfHorseshoes(){
	// accumulator for number of horseshoes:
	long long int horseshoes = 0;
	// Runs through the walk:
	for (int i=3; i<pathSize; i++){
		// Checks all 4 possiblities for a horseshoe to exist.
		// If points are in the same x:
		if (path[i-3].first == path[i].first){
			// Checks if y coordinates are within distance 1:
			if ( (path[i-3].second == (path[i].second-1)) || (path[i-3].second == (path[i].second+1)) ){
				// Increments number of horseshoes:
				horseshoes++;
			}
		// If points are in the same y:
		} else if (path[i-3].second == path[i].second){
			// Checks if x coordinates are within distance 1:
			if ( (path[i-3].first == (path[i].first-1)) || (path[i-3].first == (path[i].first+1)) ){
				// Increments number of horseshoes:
				horseshoes++;
			}
		}
	}
	// Returns the number of horseshoes:
	return horseshoes;
}

// Returns the end to end squared distance of the walk:
double endToEndDistance(){
	return pow(path[pathSize-1].first,2) + pow(path[pathSize-1].second,2);
}

// Initializes enumeration scenario:
void enumerateWrapper(){
	// Adds point (0,0) to path:
	path.push_back(node(0,0));
	// Adds it to hash table:
	hashTable.insert(make_pair(ptos(path[0]),true));
	// Variable to hold result:
	long long int resultEnum = 0;
	// Start recursion:
	_enumerate(&resultEnum, numOfHorseshoes);
	// Print result:
	cout << resultEnum*1.0/44100 << endl;
}

// Enumerates all SAWs with N steps, executes function f at each SAW and saves the sum to result:
template <class T>
void _enumerate(T* result, T (*f)(), int it){
	// Checks if end has been reached:
	if (it == pathSize){
		// Evaluates f on current SAW:
		*result += f();
		// Returns to previous iteration:
		return;
	// Tries all possiblities:
	} else {
		// Declares placeholder node:
		node next;
		// Go east:
		next = node(path[it-1].first+1, path[it-1].second);
		// Checks if collision occurred:
		if (hashTable.count(ptos(next)) == 0){
			// Adds to path:
			path.push_back(next);
			// Adds to hashtable for collision detection:
			hashTable.insert(make_pair(ptos(next),true));
			// Go to next iteration:
			_enumerate(result, f, it+1);
			// Removes element from path:
			path.pop_back();
			// Removes element from hash table:
			hashTable.erase(ptos(next));
		}
		// Go west:
		next = node(path[it-1].first-1, path[it-1].second);
		// Checks if collision occurred:
		if (hashTable.count(ptos(next)) == 0){
			// Adds to path:
			path.push_back(next);
			// Adds to hashtable for collision detection:
			hashTable.insert(make_pair(ptos(next),true));
			// Go to next iteration:
			_enumerate(result, f, it+1);
			// Removes element from path:
			path.pop_back();
			// Removes element from hash table:
			hashTable.erase(ptos(next));
		}
		// Go north:
		next = node(path[it-1].first, path[it-1].second+1);
		// Checks if collision occurred:
		if (hashTable.count(ptos(next)) == 0){
			// Adds to path:
			path.push_back(next);
			// Adds to hashtable for collision detection:
			hashTable.insert(make_pair(ptos(next),true));
			// Go to next iteration:
			_enumerate(result, f, it+1);
			// Removes element from path:
			path.pop_back();
			// Removes element from hash table:
			hashTable.erase(ptos(next));
		}
		// Go south:
		next = node(path[it-1].first, path[it-1].second-1);
		// Checks if collision occurred:
		if (hashTable.count(ptos(next)) == 0){
			// Adds to path:
			path.push_back(next);
			// Adds to hashtable for collision detection:
			hashTable.insert(make_pair(ptos(next),true));
			// Go to next iteration:
			_enumerate(result, f, it+1);
			// Removes element from path:
			path.pop_back();
			// Removes element from hash table:
			hashTable.erase(ptos(next));
		}
	}
}


// Checks if [pivot+1, n-1] elements, under transfomation matrix, collide with [0, pivot] elements. If not, copies them to the current path:
bool checkCollision(int pivot, vector<int>* matrix){
	// Placeholder vector to hold new [pivot+1, n-1] coordinates:
	vector<node> placeholder;
	// Fills placeholder vector with newly calculated coordinates while checking for collisions:
	for (int i=pivot+1; i<pathSize; i++){
		node newCoord(0,0);
		// Subtracts pivot from coordinate (takes pivot as origin):
		newCoord.first = path[i].first - path[pivot].first;
		newCoord.second = path[i].second - path[pivot].second;
		// Applies transformation:
		int a = (newCoord.first * matrix->at(0)) + (newCoord.second * matrix->at(2));
		int b = (newCoord.first * matrix->at(1)) + (newCoord.second * matrix->at(3));
		newCoord.first = a;
		newCoord.second = b;
		// Adds pivot back to coordinate (restores position):
		newCoord.first = newCoord.first + path[pivot].first;
		newCoord.second = newCoord.second + path[pivot].second;
		// If it hasn't collided:
		if (hashTable.count(ptos(newCoord)) == 0){
			// Adds to the placeholder vector:
			placeholder.push_back(newCoord);
		// If it has collided:
		} else {
			// Ends function and returns true (collision detected):
			return true;
		}
	}
	// If no collisions were detected, copies placeholder vector to [pivot+1, n-1] elements of path:
	for (int i=pivot+1; i<pathSize; i++){
		// Copies first element of placeholder to pivot+1 element of path:
		path[i] = placeholder[i-pivot-1];
	}
	// Returns false (no collision detected):
	return false;
}

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
