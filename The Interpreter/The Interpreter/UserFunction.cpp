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

vector<string> UserFunction::getDefinition() {
	return definition;
}

void UserFunction::setParameters(vector<string> parameters_) {
	for (int x = 0; x < parameters_.size(); x++) {
		parameters[x] = parameters_[x];
	}
}

void UserFunction::setParameters(string s) {
	parameters[0] = s;
}

void UserFunction::setArguments(vector<string> arguments_) {
	for (int x = 0; x < arguments_.size(); x++) {
		arguments[x].first = arguments_[x];
	}
}

void UserFunction::setArguments(vector<double> arguments_) {
	for (int x = 0; x < arguments_.size(); x++) {
		arguments[x].second = arguments_[x];
	}
}

map<int, pair<string, double>> UserFunction::getArguments() {
	return arguments;
}

map<int, string> UserFunction::getParameters() {
	return parameters;
}

void UserFunction::setReturnValue(double d){
	returnValue = d;
}

double UserFunction::getReturnValue(){
	return returnValue;
}