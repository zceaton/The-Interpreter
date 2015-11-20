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
		parameters[x].first = parameters_[x];
	}
}

void UserFunction::setParameters(string s) {
	parameters[0].first = s;
}

void UserFunction::setArguments(vector<double> arguments_) {
	for (int x = 0; x < arguments_.size(); x++) {
		parameters[x].second = arguments_[x];
	}
}

double UserFunction::call(/*vector<double> parameters, ofstream& outputFile*/) {
	/*int d = 0;
	cout << "CALLED" << endl;
	for (int x = 0; x < definition.size(); x++) {
		string toPrint = "";

		switch (getLineType(definition[x])) {
		case(BLANK_LINE) :
			//do nothing for blank line
			break;

		case(DOC_WRITE) :
			d = definition[x].find_first_of('d');
			definition[x] = definition[x].substr(d);
			if (definition[x][15] == '"') {
				for (int y = 16; y < definition[x].length() - 2; y++) {
					toPrint += definition[x][y];
				}
				outputFile << toPrint;
			}
			else {
				int y = 15;
				while (definition[x][y] != ')') {
					toPrint += definition[x][y];
					x++;
				}
				cout << toPrint << endl;
				//outputFile << variableMap[toPrint];
			}

			break;
		}
	}*/
	return returnValue;
}

void UserFunction::setReturnValue(double d){
	returnValue = d;
}

double UserFunction::getReturnValue(){
	return returnValue;
}