#ifndef SIMULATIONHANDLER_H
#define SIMULATIONHANDLER_H
#include "Handler.h"
#include "Simulation.h"
using namespace std;
/** Handler for the Simulation class, interested in ensuring that the Simulation is able to simulate as much as possible while 
	allowing the rest of the program to communicate with it.

	@author Cameron Shepherd
*/
class SimulationHandler : public Handler
{
public:
	/** Constructor 1. Default constructor.*/
	SimulationHandler();
	
	/** Constructor 2. Set the name, set up the simulation and set up event passing connection between this and creator. 
		@param name: name of this SimulationHandler
		@param creator: name of the creator
		@param _outputQueue: communication queue for the creator
	*/
	SimulationHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue);
	
	/** Default destructor. Shut down the Simulation and delete self.*/
	~SimulationHandler();
	
	/** Definition of the Handler-specific update function @see Handler.update(). This performs the update functionality for this class.*/
	void updateH();
	
	/** Get frame limit of the simulation.*/
	int getNumFrames();
	
	/** Get the rules of the simulation.*/
	weak_ptr<Rule> getRules();
	
	/** Set the simulation running. */
	void run();

private:
	
	/** Definition of the Handler-specific handleEvent function @see Handler.handleEvent(). This performs the event handling functionality for this class.*/
	void handleEventH(SEvent);
	
	/** Address of the encapsulated Simulation. */
	unique_ptr<Simulation> _theSimulation;
	
	/** Extent of frames that the user/program requires to be simulated.*/
	int targetFrame;
	
	/* Get the string representation of the simulation, ready for saving.*/
	string getSimulationString();
	
	/** Set this class to its default state.*/
	void setToDefault();

	/** Change the rules of the simulation.
		@param newRules: The new rules to govern the simulation.
	*/
	void changeRule(weak_ptr<Rule> newRules);

	/** Replace the initial frame of the simulation for a new one.
		@param newStart: The new initial layout file.
	*/
	void changeStartFrame(stringstream* newStart);

	/** Replace the initial frame of the simulation for a new one.
		@param startFrame: The new initial frame.
	*/
	void changeStartFrame(weak_ptr<vector<vector<vector<int>>>> startFrame);

	/** Replace the initial frame of the simulation for a new one.
		@param newStart: The new initial layout file in string format.
	*/
	void changeStartFrame(string newStart);
};

#endif
