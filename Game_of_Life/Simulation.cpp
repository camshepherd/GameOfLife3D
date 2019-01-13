#include "Simulation.h"
#include "nonNegative.h"

/*
TODO: Fiser's CUDA implementation
*/

Simulation::Simulation(): Simulation(10){
	//default constructor for Simulation
	// generate a 10x10 Universe
}

Simulation::Simulation(int Worldextent): Worldextent(Worldextent) {
	// overload for the Simulation constructor
	// create a Simulation of edge dimensions Worldextent
	// create it with an initial, blank frame
	_Universe = unique_ptr<vector<shared_ptr<vector<vector<vector<int>>>>>>(new vector<shared_ptr<vector<vector<vector<int>>>>>());
	newFrame();
	makeRandomStart();
	// generate new ruleset, ready for setting to some other value from elsewhere
	_Ruleset = shared_ptr<Rule>(new Rule());
	step();
}

Simulation::Simulation(string stringForm) {
	try {
		if (fromString(stringForm)) {
			// generate new ruleset, ready for setting to some other value from elsewhere
			_Ruleset = shared_ptr<Rule>(new Rule());
			step();
		}
		else {
			newFrame();
			makeRandomStart();
			// generate new ruleset, ready for setting to some other value from elsewhere
			_Ruleset = shared_ptr<Rule>(new Rule());
			step();
		}
	}
	catch (const char* e) {
		valid = false;
		cout << "Error when reading the string, exception: " << e << endl;
	}
}

Simulation::Simulation(weak_ptr<vector<vector<vector<int>>>> Storeduniverse, int Worldextent) : Simulation(Worldextent) {
	// Constructor for Simulation
	// Create an initial Simulation state based off of the given stored universe
	
	//TODO: actually chuck the stored universe into the _Universe
	// and also make it do something here
}

Simulation::~Simulation()
{
	// Default destructor thing or something
}

bool Simulation::newFrame() {
	// add a new blank frame onto the end of _Universe
	vector<vector<vector<int>>>* v = new vector<vector<vector<int>>>(Worldextent, vector<vector<int>>(Worldextent, vector<int>(Worldextent, 0)));

	_Universe.get()->push_back(shared_ptr<vector<vector<vector<int>>>>(v));
	return true;
}

bool Simulation::step() {
	// steps on the simulation one frame further than the current
	// _Universe is zero-indexed so size() gives ref of the new frame
	int time = _Universe->size();
	newFrame();
	for (int x = 0; x < Worldextent; x++) {
		for (int y = 0; y < Worldextent; y++) {
			for (int z = 0; z < Worldextent; z++) {
				setCell(time, x, y, z, _Ruleset->nextState(getCell(time - 1, x, y, z), countNeighbours(time - 1, x, y, z)));
			}
		}
	}
	return true;
}

bool Simulation::setCell(int time, int x, int y, int z, int val) {
	//set the specified cell value to that of val
	if (nonNegative(time, x, y, z)) {
		if (framesSimulated() > time) {
			if (_Universe->at(time)->size() > x){
				if (_Universe->at(time)->at(x).size() > y) {
					if (_Universe->at(time)->at(x)[y].size() > z) {
						_Universe->at(time)->at(x)[y][z] = val;
						return true;
					}
				}
			}
		}
	}
	return false;
}

int Simulation::getCell(int time, int x, int y, int z) {
	//Get the value of the specified cell
	if (nonNegative(time, x, y, z)) {
		if (framesSimulated() > time) {
			if (_Universe->at(time)->size() > x) {
				if (_Universe->at(time)->at(x).size() > y) {
					if (_Universe->at(time)->at(x).at(y).size() > z) {
						return _Universe->at(time)->at(x)[y][z];
					}
				}
			}
		}
	}
	return -200;
	//failure
}

int Simulation::framesSimulated() {
	//return how many frames have been simulated
	return this->_Universe->size();
}

int Simulation::getEdgeLength() {
	//get the length of each edge of the cube-shaped world
	return this->Worldextent;
}

bool Simulation::setEdgeLength(int Edgelength) {
	// set the length of each edge of the cube-shaped world
	Worldextent = Edgelength;
	return true;
};

int Simulation::countNeighbours(int time, int arg_x, int arg_y, int arg_z) {
	//count how many live neighbours there are to the specified cell
	//return 1;
	int Neighbourcount = 0;
	if (framesSimulated() <= time || time < 0){
		//failure
	}
	// for every adjacent cell in the frame, 
	for (int x = arg_x - 1; x <= arg_x + 1 && x < Worldextent; x++) {
		for (int y = arg_y - 1; y <= arg_y + 1 && y < Worldextent; y++) {
			for (int z = arg_z - 1; z <= arg_z + 1 && z < Worldextent; z++) {
				if (x >= 0 && y >= 0 && z >= 0){
					if (_Universe->at(time)->at(x)[y][z] >= 1) {
						//make sure that the target cell isn't counted
						if (!(x == arg_x && y == arg_y && z == arg_z)) {
							Neighbourcount += 1;
						}
					}
				}
			}
		}
	}

	return Neighbourcount;
}


bool Simulation::simulateToFrame(int n) {
	// simulate all steps up to the target frame
	while (_Universe->size() <= n) {
		step();
	}
	return true;
}

bool Simulation::changeRule(string rule, int newVal) {
	// change the stored rules governing the simulation
	return _Ruleset->changeRule(rule, newVal);
}

bool Simulation::changeRule(weak_ptr<Rule> newRules){
	// change the stored rules governing the simulation to some new Rule object
	try {
		if (shared_ptr<Rule>(newRules)->Eu > -1) {
			_Ruleset->~Rule();
			_Ruleset = shared_ptr<Rule>(newRules);
			shared_ptr<vector<vector<vector<int>>>> firstFrame = _Universe->at(0);
			_Universe = unique_ptr<vector<shared_ptr<vector<vector<vector<int>>>>>>(new vector<shared_ptr<vector<vector<vector<int>>>>>());
			_Universe->push_back(firstFrame);
			return true;
		}
		else {
			cout << "Given empty Rule" << endl;
			return false;
		}
	}
	catch (const char* e) {
		cout << "Caught exception: " << e << endl;
		getchar();
	}
}

weak_ptr<Rule> Simulation::getRules() {
	// get the rules governing the simulation
	return _Ruleset;
}

weak_ptr<vector<vector<vector<int>>>> Simulation::getFrame(int n) {
	// get target frame
	int count = 0;
	return weak_ptr<vector<vector<vector<int>>>>(_Universe->at(n));
}

int Simulation::getUniverseSize() {
	// get the size of the universe
	return _Universe->size();
}

string Simulation::toString(){
	// convert the simulation to a string representation for printing to file
	return toString(0);
}

string Simulation::toString(int targetFrame){
	// begin with triplet of edge lengths
	string theString = "" + std::to_string(getEdgeLength()) + "," + std::to_string(getEdgeLength()) + "," + std::to_string(getEdgeLength()) + ",\n";
	//output triplets, representing the cells that are alive only in (x,y,z)
	for (int x = 0; x < getEdgeLength(); x++) {
		for (int y = 0; y < getEdgeLength(); y++) {
			for (int z = 0; z < getEdgeLength(); z++) {
				if (getCell(targetFrame, x, y, z) > 0) {
					theString += std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ",\n";
				}
			}
		}
	}
	return theString;
}


bool Simulation::fromString(string stringUniverse) {
	// Turn csv to vector of ints, get it working from there
	std::stringstream ss(stringUniverse);
	vector<int> data;
	int i, a, b, c;
	while (ss >> i)
	{
		data.push_back(i);
		int thing = i;
		if (ss.peek() == ',' || ss.peek() == ' ')
			ss.ignore();
	}
	if (data.size() >= 3 && data.size() % 3 == 0) {
		Worldextent = data[0];
		int thing = data[0];
		_Universe.reset();
		_Universe = unique_ptr<vector<shared_ptr<vector<vector<vector<int>>>>>>(new vector<shared_ptr<vector<vector<vector<int>>>>>());
		newFrame();
		// For each cell, set to state new.
		for (int ref = 0; ref < int(data.size() / 3); ref++) {
			a = data[(3 * ref)];
			b = data[(3 * ref) + 1];
			c = data[(3 * ref) + 2];
			if (a < Worldextent && b < Worldextent && c < Worldextent) {
				_Universe->at(0)->at(a)[b][c] = 2;
			}
		}
		return true;
	}
	else {
		return false;
	}
}


bool Simulation::makeRandomStart() {
	// Randomly initialise the cell values to a primordial soup configuration
	int x, y, z;
	for (long long k = 0; k < (Worldextent*Worldextent*Worldextent); k++) {
		x = int((Worldextent - 1)* (float)rand() / (float)(RAND_MAX / 1.0f));
		y = int((Worldextent - 1)* (float)rand() / (float)(RAND_MAX / 1.0f));
		z = int((Worldextent - 1) * (float)rand() / (float)(RAND_MAX / 1.0f));
		_Universe->at(0)->at(x)[y][z] = 2;
	}
	return true;
}
