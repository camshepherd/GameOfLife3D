#include "Rule.h"
Rule::Rule()
{
	El = 1;
	Eu = 9;
	Fl = 2;
	Fu = 12;
}

Rule::Rule(int El, int Eu, int Fl, int Fu) : El(El), Eu(Eu), Fl(Fl), Fu(Fu) {};

Rule::Rule(string stringFormat) {
	std::stringstream ss(stringFormat);
	vector<int> vec;
	int i;
	while (ss >> i)
	{
		vec.push_back(i);
		if (ss.peek() == ',' || ss.peek() == ' ')
			ss.ignore();
	}
	if (vec.size() >= 4) {
		El = vec[0];
		Eu = vec[1];
		Fl = vec[2];
		Fu = vec[3];
	}
	else {
		El = -1;
		Eu = -1;
		Fl = -1;
		Fu = -1;
	}
}

signed int Rule::nextState(int curr_state, int adjacent_count) {
	/*
		RULES:
		2 = New
		1 = alive
		0 = never was alive
		-n = dead for n-1 turns (-1 = just died) (minimum = -20)
		-100 = currently selected cell-dead
		-101 = currently selected cell-alive
	*/
	//std::cout << "Next state being evaluated" << endl;
	switch (curr_state) {
	case 2:
		//new
		if (adjacent_count >= El && adjacent_count <= Eu) {
			return 1;
		}
		else {
			return -1;
		}
	case 1:
		//alive
		if (adjacent_count >= El && adjacent_count <= Eu) {
			return 1;
		}
		else {
			return -1;
		}
	case -100:
		//selected cell-dead
		return -100;
	case -101:
		//selected cell-alive
		return -101;
	default:
		if (adjacent_count >= Fl && adjacent_count <= Fu) {
			// birth
			return 2;
		}
		else if (curr_state < 0) {
			if (curr_state > -20) {
				return curr_state - 1;
			}
			else {
				return -20;
				// -20 being the limit of turns stored since death
			}
		}
		else if (curr_state == 0) {
			// cell was never alive
			return 0;
		}
	}
}

Rule::~Rule()
{
}


string Rule::toString() {
	string stringRep = std::to_string(El) + "," + std::to_string(Eu) + "," + std::to_string(Fl) + "," + std::to_string(Fu) + ".\n";
	return stringRep;
}


bool Rule::changeRule(string ruleName, int newVal) {
	if (ruleName == "El") {
		if (El != newVal) {
			El = newVal;
			return true;
		}
	}
	else if (ruleName == "Eu") {
		if (Eu != newVal) {
			Eu = newVal;
			return true;
		}
	}
	else if (ruleName == "Fl") {
		if (Fl != newVal) {
			Fl = newVal;
			return true;
		}
	}
	else if (ruleName == "Fu") {
		if (Fu != newVal) {
			Fu = newVal;
			return true;
		}
	}
	return false;
}

int Rule::getRule(string ruleName) {
	if (ruleName == "Fl") {
		return Fl;
	}
	else if (ruleName == "Fu") {
		return Fu;
	}
	else if (ruleName == "El") {
		return El;
	}
	else if (ruleName == "Eu") {
		return Eu;
	}
}