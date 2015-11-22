#include "UserFunction.h"
#include <vector>
#include <iostream>

void UserFunction::setDefinition(vector<string> definition_) {
	definition = definition_;
}

vector<string> UserFunction::getDefinition() {
	return definition;
}

void UserFunction::setParameters(vector<string> parameters_) {
	parameters = parameters_;
}

void UserFunction::setParameters(string s) {
	parameters[0] = s;
}

vector<string> UserFunction::getParameters() {
	return parameters;
}

void UserFunction::setReturnValue(double d) {
	returnValue = d;
}

double UserFunction::getReturnValue() {
	return returnValue;
}