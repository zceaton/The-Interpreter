#include "UserFunction.h"
#include <vector>
#include <iostream>

void UserFunction::setDefinition(vector<string> definition_){
	definition = definition_;
	cout << "THE DEFINITION:" << endl;
	for (int x = 0; x < definition.size(); x++) {
		cout << definition[x] << endl;
	}
}

void UserFunction::setParameters(vector<string> parameters_){
	parameters = parameters_;
	/*cout << "THE PARAMETER LIST: " << endl;
	if (parameters.size() != 0) {
		for (int x = 0; x < definition.size(); x++) {
			cout << parameters[x] << ",";
		}
		cout << endl;
	}*/
}

double UserFunction::call(vector<double> parameters) {

}