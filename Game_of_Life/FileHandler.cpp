#include "FileHandler.h"


FileHandler::FileHandler()
{
}

FileHandler::FileHandler(string name, string creator, weak_ptr<queue<SEvent>> _outputQueue): Handler(name, creator, _outputQueue) {
	/** Default rule directory path*/
	defaultRuleDirectory = "./Saves/Rules";

	/** Default universe directory path*/
	defaultUniverseDirectory = "./Saves/Universe";

	/** Default settings directory path*/
	defaultSettingsDirectory = "./Saves/Settings";
}


FileHandler::~FileHandler()
{
}

void FileHandler::updateH() {
	if (closing) {
		running = false;
	}
}

void FileHandler::handleEventH(SEvent theEvent) {
	if (theEvent._sType == FILE_NAMES_GET2) {
		broadcastMessage(SEvent(name, FILE_NAMES2).fileType(theEvent._fileType).vectorString(getFileNames(theEvent._fileType)));
	}
	else if (theEvent._sType == FILE_WRITE2) {
		sendMessage(SEvent(name, FILE_WRITTEN2).fileType(theEvent._fileType).operationState(saveFile(theEvent._name, theEvent._fileType, theEvent._str1)), "IntrepidLemming");
		sendMessage(SEvent(name, FILE_NAMES2).fileType(theEvent._fileType).vectorString(getFileNames(theEvent._fileType)), "IntrepidLemming");
	}
	else if (theEvent._sType == FILE_OPEN) {
		broadcastMessage(SEvent(name, FILE_OPENED).fileType(theEvent._fileType).str1(readFile(theEvent._name, theEvent._fileType)).operationState(opSuccessfulness));
		broadcastMessage(SEvent(name, FILE_NAMES2).fileType(theEvent._fileType).vectorString(getFileNames(theEvent._fileType)));
	}
	else if (theEvent._sType == FILE_DELETE) {
		broadcastMessage(SEvent(name, FILE_DELETED).fileType(theEvent._fileType).operationState(deleteFile(theEvent._name, theEvent._fileType)));
		broadcastMessage(SEvent(name, FILE_NAMES2).fileType(theEvent._fileType).vectorString(getFileNames(theEvent._fileType)));
	}
}

OperationState FileHandler::saveFile(string fileName, FileType fileType, const string& givenText) {
	string dir;
	if (fileType == FT_RULES) {
		dir = defaultRuleDirectory;
	}
	else if (fileType == FT_UNIVERSE) {
		dir = defaultUniverseDirectory;
	}
	boost::filesystem::ofstream outputFile(path(dir + "/" + fileName + ".csv"),ios::trunc);
	try {
		outputFile << givenText << endl;
		outputFile.close();
		return OP_SUCCESS;
	}
	catch(char* e) {
		cout << e << "stuhig" << endl;
	}
	return OP_SUCCESS;
}

OperationState FileHandler::deleteFile(string fileName, FileType fileType) {
	string preceding = "";
	if (fileType == FT_RULES) {
		preceding = "./Saves/Rules/";
	}
	else if (fileType == FT_UNIVERSE) {
		preceding = "./Saves/Universe/";
	}
	try {
		directory_entry file(complete(preceding + fileName + ".csv", current_path()));
		//cout << "path steam: " << file.path().stem() << endl;
		if (is_regular_file(file.status()) && file.path().stem() == fileName) {
			remove(file.path());
			return OP_SUCCESS;
		}
	}
	catch (const char* e) {
		cout << "Failed when deleting the file or something" << endl;
	}
	return OP_FAIL;
}

string FileHandler::readFile(string fileName, FileType fileType) {
	opSuccessfulness = OP_FAIL;
	path directory_path;
	string line;
	fileString = "";
	string preceding = "";
	if (fileType == FT_RULES) {
		preceding = "./Saves/Rules/";
	}
	else if (fileType == FT_UNIVERSE) {
		preceding = "./Saves/Universe/";
	}
	try {
		boost::filesystem::ifstream subjectFile(complete(preceding + fileName + ".csv", current_path()));
		if (subjectFile.is_open()) {
			while (subjectFile >> line) {
				fileString += line;
			}
			subjectFile.close();
			opSuccessfulness = OP_SUCCESS;
		}
		else {
			cout << "Abject failure" << endl;
		}
	}
	catch(exception e) {
		cout << "Failed to open the file or something" << endl;
	}
	return fileString;
}


vector<string> FileHandler::getFileNames(FileType fileType) {
	vector<string> fileNames = vector<string>();
	directory_iterator dir;
	if (fileType == FT_RULES) {
		dir = directory_iterator(complete("./Saves/Rules", current_path()));
	}
	else if (fileType == FT_UNIVERSE) {
		dir = directory_iterator(complete("./Saves/Universe", current_path()));
	}
	while (dir != directory_iterator{}) {
		if (is_regular_file(dir->status()) && dir->path().extension() == ".csv") {
			fileNames.push_back(dir->path().stem().string());
		}
		dir++;
	}
	return fileNames;
}
