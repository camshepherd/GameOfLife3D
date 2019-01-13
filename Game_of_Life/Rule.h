#ifndef RULE_H
#define RULE_H
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
/** A straightforward store of rules to govern a simulation.
	@author Cameron Shepherd
*/
class Rule
{
public:
	Rule();
	Rule(int, int, int, int);
	Rule(string);
	~Rule();
	signed int nextState(int, int);
	string toString();
	int getRule(string ruleName);
	bool changeRule(string ruleName, int newVal);
	int El;
	int Eu;
	int Fl;
	int Fu;
};
#endif
