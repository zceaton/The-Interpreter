#ifndef CSE250_FALL2015_USERFUNCTION_H
#define CSE250_FALL2015_USERFUNCTION_H

#include <vector>
#include <string>
#include "Parser.h"
#include <fstream>

using namespace std;

// This class is not required, but might be helpful in writing your interpreter
class UserFunction {

private:
	vector<string> definition, parameters;

public:
	void setDefinition(vector<string>);
	vector<string> getDefinition();
	void setParameters(vector<string>);
	void setParameters(string);
	double call(ofstream & outputFile);
	double call(/*vector<double> parameters*/);
	void setReturnValue(double);
	double getReturnValue();
	int returnValue;
};

#endif //CSE250_FALL2015_USERFUNCTION_H