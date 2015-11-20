#ifndef UNTITLED_INTERPRETER_H
#define UNTITLED_INTERPRETER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "UserFunction.h"
#include "Parser.h"

using namespace std;

/*enum CHAR_TYPE;
enum OPERATOR_TYPE;*/

class Interpreter {

private:
	// suggestions:
	map<string, double> variableMap; // a map of variable names to Variable values
	map<string, UserFunction> functionMap; // a map of user defined functions
										   // stack to remember nesting of if blocks

public:
	enum CHAR_TYPE {
		DIGIT, // 0: A number (0-9)
		L_PARENTHESIS, // 1: A left parenthesis '('
		R_PARENTHESIS, // 2: A right parenthesis ')'
		HIGH_PRIORITY, // 3: A multiplication or division sign (*, /)
		LOW_PRIORITY, // 4: An addition or subtraction sign (+, -)
		VARIABLE, //5: a variable (a-z)
		NOT_VALID //6: Anything else
	};
	enum OPERATOR_TYPE {
		MULTIPLICATION,
		ADDITION,
		SUBTRACTION,
		DIVISION,
		NOT_OPERATOR
	};
	void interpretScript(ifstream&, ofstream&);
	double computeInfix(string); // separate function for computing infix notation
	vector<string> infixToPostfix(string infixExpression);
	string correctSpacing(string original);
	void interpretLine(string s, ifstream & inputFile, ofstream & outputFile);
	double evaluateFunction(string line, ofstream & outputFile);
	/*CHAR_TYPE getCharType(string s);
	OPERATOR_TYPE getOperatorType(string s);*/
};

#endif //UNTITLED_INTERPRETER_H