#include "UserFunction.h"
#include <vector>
#include <iostream>

void UserFunction::setDefinition(vector<string> definition_) {
	definition = definition_;
	cout << "THE DEFINITION:" << endl;
	for (int x = 0; x < definition.size(); x++) {
		cout << definition[x] << endl;
	}
}

void UserFunction::setParameters(vector<string> parameters_) {
	parameters = parameters_;
	cout << "THE PARAMETER LIST: " << endl;
	for (int x = 0; x < parameters.size(); x++) {
		cout << parameters[x] << endl;
	}
}

void UserFunction::setParameters(string s) {
	parameters.push_back(s);
	cout << "THE PARAMETER LIST: " << endl;
	cout << parameters[0] << endl;
}

double UserFunction::call(/*vector<double> parameters*/) {
	cout << "CALLED" << endl;
	return 0.0;
}