#pragma once
#include "Handler.h"
#include <boost\filesystem.hpp>
using namespace boost::filesystem;

/** Standarised access point for the various user preset files. This class has the functionality available for all standard operations on all the
	save files required by the user: saving, loading and deletion. It also has the functionality available to handle the program settings file.
	
	@author Cameron Shepherd
	*/
class FileHandler : public Handler {
public:
	/** Constructor 1: Default constructor.*/
	FileHandler();

	/** Constructor 2: Standard thing.*/
	FileHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue);

	/** Default destructor. */
	~FileHandler();


	/** Definition of the Handler-specific update function @see Handler.update(). This performs the update functionality for this class.
	*/
	void updateH();


protected:
	/** Save the given file.
		@param name: the preset name
		@param fileType: the type of the file being saved.
	*/
	OperationState saveFile(string name, FileType fileType, const string& givenText);


	/** Delete the specified file.
		@param name: the preset name
		@param fileType: the type of the file being deleted
	*/
	OperationState deleteFile(string name, FileType fileType);


	/** Read the specified file.
		@param name: the preset name
		@param fileType: the type of the file being read
		@return the object saved in the file
	*/
	string readFile(string name, FileType fileType);


	/** Read the specified file.
		@param fileType: the type of the files being evaluated
		@return the names of the files of the specified FileType
	*/
	vector<string> getFileNames(FileType fileType);


	/** Take any non-generic action as a result of reading the message. @see Handler.handleEventH().
		@param theEvent: The message to be handled
	*/
	void handleEventH(SEvent theEvent);


	/** Default rule directory path*/
	string defaultRuleDirectory = "./Saves/Rules";

	/** Default universe directory path*/
	string defaultUniverseDirectory = "./Saves/Universe";

	/** Default settings directory path*/
	string defaultSettingsDirectory = "./Saves/Settings";

	/** Stored copy of the opened file, temp version, not for long term use */
	string fileString;

	/** Successfulness of the most recent operation */
	OperationState opSuccessfulness;
};
