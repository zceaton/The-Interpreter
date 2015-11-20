#ifndef CSE250_FALL2015_USERFUNCTION_H
#define CSE250_FALL2015_USERFUNCTION_H

#include <vector>
#include <string>
#include "Parser.h"
#include <fstream>
#include <map>

using namespace std;

// This class is not required, but might be helpful in writing your interpreter
class UserFunction {

private:
	vector<string> definition;
	map<int, string> parameters;
	map<int, pair<string, double>> arguments;

public:
	void setDefinition(vector<string>);
	vector<string> getDefinition();
	void setParameters(vector<string>);
	void setParameters(string);
	void setArguments(vector<string> arguments_);
	void setArguments(vector<double> arguments_);
	map<int, pair<string, double>> getArguments();
	map<int, string> getParameters();
	void setReturnValue(double);
	double getReturnValue();
	int returnValue;
};

#endif //CSE250_FALL2015_USERFUNCTION_H