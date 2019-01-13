#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include "Rule.h" 
#include <iostream>
#include <sstream>
#include <random>
#include <memory>
using namespace std;
/** Class to handle the simulation of frames in a universe according to some stored set of rules.
	@author Cameron Shepherd
*/
class Simulation
{
public:

	/** Constructor 1. Generate Universe 10x10 */
	Simulation();
	
	/** Constructor 2. Generate Universe with specified edge lengths.
		@param edgeLength: Edge length of the universe.
	*/
	Simulation(int edgeLength);

	/** Constructor 3. Generate universe from given start frame, with given dimensions.
		@param startFrame: The initial frame of the simulation.
		@param edgeLength: Edge length of the universe.
	*/
	Simulation(weak_ptr<vector<vector<vector<int>>>> startFrame, int edgeLength);

	/** Constructor 4. Generate universe from given string representation of first frame.
		@param stringForm: The string representation of the universe.
	*/
	Simulation(string stringForm);

	/** Default destructor.*/
	~Simulation();

	/** Convert Simulation to a string representation, ready for saving to a file.*/
	string toString();

	/** Convert specific frame in the simulation to a string representation, ready for saving to a file.
		@param targetFrame: The zero-indexed reference for the target frame.*/
	string toString(int targetFrame);

	/** Get the specified frame.
		@param frameRef: The zero-indexed reference for the target frame.
	*/
	weak_ptr<vector<vector<vector<int>>>> getFrame(int frameRef);

	/** Get the state of the specified cell.
		@param time: zero-indexed frame location
		@param x: x location
		@param y: y location
		@param z: z location
	*/
	int getCell(int time, int x, int y, int z);

	/** Simulate an additional frame.*/
	bool step();

	/** Get the number of frames availabe.*/
	int framesSimulated();

	/** Count the number of neighbours of the specified cell.
		@param time: zero-indexed frame location
		@param x: x location
		@param y: y location
		@param z: z location
	*/
	int countNeighbours(int time, int x, int y, int z);

	/** Set the value of the specified cell.
		@param time: zero-indexed frame location
		@param x: x location
		@param y: y location
		@param z: z location
		@param val: The value to set the specified cell to 
	*/
	bool setCell(int time, int x, int y, int z, int val);

	/** Get the edge length of the universe.*/
	int getEdgeLength();

	/** Set the edge length of the universe.
		@param edgeLength: The new edge length of the universe.
	*/
	bool setEdgeLength(int edgeLength);

	/** Simulate frames in the universe until the specified frame is reached.
		@param frameNum: The target frame.
	*/
	bool simulateToFrame(int frameNum);

	/** Get the number of frames in the universe.*/
	int getUniverseSize();

	/** Change the specified rule for the Simulation.
		@param rule: The string identifier for the rule to be changed.
		@param newVal: The new value for the specified rule.
	*/
	bool changeRule(string rule, int newVal);

	/** Swap the rules for the Simulation.
		@param newRules: The new rules.
	*/
	bool changeRule(weak_ptr<Rule> newRules);

	/** Get the rules used to govern the Universe.*/
	weak_ptr<Rule> getRules();

	/** Set the universe from the given string form.
		@param stringUniverse: The string representation of the universe.
	*/
	bool fromString(string stringUniverse);
	bool valid;
	
private:
	/** The rules used to govern the ruleset.*/
	shared_ptr<Rule> _Ruleset;
	
	/** The edge length of the universe.*/
	int Worldextent;
	
	/** Set the initial state of the Universe to be a randomly generated 'Primordial Ooze'*/
	bool makeRandomStart();

	/** The store of the universe over time. Each time contains a frame, containing the state of the Universe at that time.*/
	unique_ptr<vector<shared_ptr<vector<vector<vector<int>>>>>> _Universe;

	/** Add an additional, blank frame to the simulation.*/
	bool newFrame();
};
#endif
