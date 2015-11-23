#include "Interpreter.h"
#include <stack>

string lineFromFile, temp = "", rightSide = "", left = "", rightSide1 = "";
int lineNumber = 0, space, leftParenth, d;
vector<string> tokenizedLine, splitLine, definition, tokenizedLine1, splitLine1, argumentListV, functionParameterList, splitCondtional, conditionalDefinition;
string variableName, toPrint, functionName, parameterList, argumentList, testName, conditionalSymbol, conditionalExpression;
double variableValue;
bool evalElse;

void Interpreter::interpretScript(ifstream& inputFile, ofstream& outputFile) {
	while (std::getline(inputFile, lineFromFile)) {
		interpretLine(lineFromFile, inputFile, outputFile);
	}
	// write the result from the return statement of the program into the output file
	//outputFile << "output of the document.write lines";
}

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

OPERATOR_TYPE getOperatorType(string s);
CHAR_TYPE getCharType(string s);

double Interpreter::computeInfix(string infixExpression) {
	string corrected = correctSpacing(infixExpression);
	vector<string> postfixExpression = infixToPostfix(corrected);
	stack<double> calcStack;
	double temp1, temp2;

	/*std::cout << infixExpression << endl;
	for (int x = 0; x < postfixExpression.size(); x++) {
	std::cout << postfixExpression[x] << " ";
	}
	std::cout << endl;*/

	for (int x = 0; x < postfixExpression.size(); x++) {
		if (getOperatorType(postfixExpression[x]) == NOT_OPERATOR) {
			if (getCharType(postfixExpression[x]) == DIGIT) {
				calcStack.push(stod(postfixExpression[x]));
				//std::cout << "DIGIT: " << postfixExpression[x] << endl;
			}
			else if (getCharType(postfixExpression[x]) == VARIABLE) {
				calcStack.push(variableMap[postfixExpression[x]]);
				//std::cout << "VARIABLE: " << postfixExpression[x] << " = " << variableMap[postfixExpression[x]] <<  endl;
			}
		}

		else {
			temp1 = calcStack.top();
			calcStack.pop();
			temp2 = calcStack.top();
			calcStack.pop();
			//std::cout << temp1 << "|" << temp2 << endl;

			switch (getOperatorType((postfixExpression[x]))) {
			case (MULTIPLICATION) :
				calcStack.push(temp2 * temp1);
				//std::cout << "MULTIPLICATION" << endl;
				break;

			case(ADDITION) :
				calcStack.push(temp2 + temp1);
				//std::cout << "ADDITION" << endl;
				break;

			case(DIVISION) :
				calcStack.push(temp2 / temp1);
				//std::cout << "DIVISION" << endl;
				break;

			case(SUBTRACTION) :
				calcStack.push(temp2 - temp1);
				//std::cout << "SUBTRACTION" << endl;
				break;
			}
		}
	}
	return calcStack.top();
}

vector<string> Interpreter::infixToPostfix(string infixExpression) {
	string postfixExpression, tempString, infix = infixExpression, infix1 = "";
	vector<string> tokenized, postfixTokenized;
	stack<string> holdingStack;

	for (int x = 0; x < infix.length(); x++) {
		if (infix[x] == '(') {
			infix1 += infix[x];
			infix1 += " ";
		}
		else if (infix[x] == ')') {
			infix1 += " ";
			infix1 += infix[x];
		}
		else { infix1 += infix[x]; }
	}
	tokenized = tokenize(infix1, " ");

	for (int x = 0; x < tokenized.size(); x++) {
		switch (getCharType(tokenized[x])) {
		case DIGIT: //if the char is a digit move it to the postfix expression
			postfixTokenized.push_back(tokenized[x]);
			break;

		case VARIABLE:
			postfixTokenized.push_back(tokenized[x]);
			break;

		case L_PARENTHESIS://if the char is a left parenthesis automatically push it onto the stack
			holdingStack.push(tokenized[x]);
			break;

		case R_PARENTHESIS://if the char is a right parenthesis move all the operators between the parenthesis into the postfix expression
			while (holdingStack.top() != "(") {
				postfixTokenized.push_back(holdingStack.top());
				holdingStack.pop();
			}
			holdingStack.pop();
			break;

		case HIGH_PRIORITY:
			if (holdingStack.empty()) {
				holdingStack.push(tokenized[x]);
				break;
			}
			else {
				if (getCharType(holdingStack.top()) == LOW_PRIORITY) {
					holdingStack.push(tokenized[x]);
					break;
				}
				else if (getCharType(holdingStack.top()) == HIGH_PRIORITY) {
					postfixTokenized.push_back(holdingStack.top());
					holdingStack.pop();
					holdingStack.push(tokenized[x]);
					break;
				}
				else {
					holdingStack.push(tokenized[x]);
					break;
				}
			}

		case LOW_PRIORITY:
			if (holdingStack.empty()) {
				holdingStack.push(tokenized[x]);
				break;
			}
			else {
				if (getCharType(holdingStack.top()) == LOW_PRIORITY) {
					postfixTokenized.push_back(holdingStack.top());
					holdingStack.pop();
					holdingStack.push(tokenized[x]);
					break;
				}
				else if (getCharType(holdingStack.top()) == HIGH_PRIORITY) {
					while (!holdingStack.empty() && getCharType(holdingStack.top()) == HIGH_PRIORITY) {
						postfixTokenized.push_back(holdingStack.top());
						holdingStack.pop();
					}
					holdingStack.push(tokenized[x]);
					break;
				}
				else {
					holdingStack.push(tokenized[x]);
					break;
				}
			}
		}//end switch statement
	}

	while (!holdingStack.empty()) {
		postfixTokenized.push_back(holdingStack.top());
		holdingStack.pop();
	}

	return postfixTokenized;
}

CHAR_TYPE getCharType(string s) {
	if ((s[0] >= '0' && s[0] <= '9') || s[0] == '.') return DIGIT;
	if (s == "(") return L_PARENTHESIS;
	if (s == ")") return R_PARENTHESIS;
	if (s == "/" || s == "*") return HIGH_PRIORITY;
	if (s == "+" || s == "-") return LOW_PRIORITY;
	if ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')) return VARIABLE;
	else return NOT_VALID;
}

OPERATOR_TYPE getOperatorType(string s) {
	if (s == "*") return MULTIPLICATION;
	if (s == "+") return ADDITION;
	if (s == "-") return SUBTRACTION;
	if (s == "/") return DIVISION;
	else return NOT_OPERATOR;
}

int getSymbolIndex(string line) {
	char c;
	vector<char> symbols;
	symbols.push_back('+');
	symbols.push_back('-');
	symbols.push_back('*');
	symbols.push_back('/');

	for (int x = 0; x < line.length(); x++) {
		c = line[x];
		for (int y = 0; y < 4; y++) {
			if (c == symbols[y]) {
				return x;
			}
		}
	}
	return -1;
}

string Interpreter::correctSpacing(string original) {
	string s = "", a = "", corrected = "";
	a = original;

	for (int x = 0; x < a.length(); x++) {
		s = "";
		s += a[x];
		if (s == "," && a[x + 1] == ' ') {
			corrected += a[x];
			x++;
		}
		else if (getOperatorType(s) == DIVISION) {
			if (a[x - 1] != ' ' && a[x - 1] != 'r') {
				corrected += " ";
			}
			corrected += a[x];
			if (a[x + 1] != ' ' && a[x + 1] != '>') {
				corrected += " ";
			}
		}
		else if (getOperatorType(s) != NOT_OPERATOR || s == "=") {
			if (a[x - 1] != ' ') {
				corrected += " ";
			}
			corrected += a[x];
			if (a[x + 1] != ' ') {
				corrected += " ";
			}
		}
		else { corrected += a[x]; }
	}
	return corrected;
}

void Interpreter::interpretLine(string s, ifstream& inputFile, ofstream& outputFile) {
	lineNumber++, toPrint = "", functionName = "", parameterList = "", argumentList = "";
	LINE_TYPE lineType = getLineType(lineFromFile); // Check Parser.h for the different line types
	if (lineType != 0) {
		std::cout << "line \"" << lineFromFile << "\" is type: " << lineType << endl;
	}
	//std::cout << lineFromFile << " | ";
	lineFromFile = correctSpacing(lineFromFile);
	//std::cout << lineFromFile << endl;
	tokenizedLine = tokenize(lineFromFile, " ");
	UserFunction uf;
	// Use your interpreter to execute each line

	switch (lineType) {
	case(BLANK_LINE) :
		//do nothing for blank line
		break;
	case(DEFINE_VAR) :
		if (tokenizedLine.size() == 4 && variableMap.find(tokenizedLine[3]) == variableMap.end()) {//if it's a single number, or method call
			try {
				variableMap[tokenizedLine[1]] = stod(tokenizedLine[3]);//try to get the value if it's a number
			}
			catch (invalid_argument& e) {//if not catch the exception
				for (int x = 0; x < tokenizedLine[3].length(); x++) {//gets the function name
					if (tokenizedLine[3][x] == '(') {
						break;
					}
					functionName += tokenizedLine[3][x];
				}
				std::cout << "FUNCTION NAME: " << functionName << endl;
				for (int x = tokenizedLine[3].find_first_of('(') + 1; tokenizedLine[3][x] != ')'; x++) {//get the arguments in a string
					argumentList += tokenizedLine[3][x];
				}

				argumentListV = tokenize(argumentList, ",");//puts the arguments in a vector

				functionParameterList = functionMap[functionName].getParameters();

				for (int x = 0; x < argumentListV.size(); x++) {
					//std::cout << "Parameter: " << functionParameterList[x] << endl;
					//std::cout << "Argument: " << argumentListV[x] << endl;
					variableMap[functionParameterList[x]] = variableMap[argumentListV[x]];//inserts values of parameters/arguments in the variable map
				}

				std::cout << "Function being called: " << functionName << endl;

				for (int x = 0; x < functionMap[functionName].getDefinition().size(); x++) {//evaluates the lines of the function
					evaluateFunction(functionMap[functionName].getDefinition()[x], outputFile, functionName);
				}

				variableMap[tokenizedLine[1]] = functionMap[functionName].getReturnValue();
			}
		}
		else {
			splitLine = tokenize(lineFromFile, "=");
			rightSide = splitLine[1];
			variableMap[tokenizedLine[1]] = computeInfix(rightSide);
		}
		break;

	case(USER_DEFINED) :
		if (lineFromFile.find_first_of('=') != -1) {
			splitLine = tokenize(lineFromFile, "=");
			rightSide = splitLine[1];
			if (rightSide[0] == ' ') { rightSide = rightSide.substr(1); }
			variableMap[tokenizedLine[0]] = computeInfix(rightSide);
		}
		else {
			for (int x = 0; lineFromFile[x] != '('; x++) {
				functionName += lineFromFile[x];
			}

			for (int x = lineFromFile.find_first_of('(') + 1; lineFromFile[x] != ')'; x++) {//get the arguments in a string
				argumentList += lineFromFile[x];
			}
			cout << "ARGUMENTLIST: " << argumentList << endl;

			argumentListV = tokenize(argumentList, ",");
			functionParameterList = functionMap[functionName].getParameters();

			for (int x = 0; x < argumentListV.size(); x++) {
				variableMap[functionParameterList[x]] = variableMap[argumentListV[x]];//inserts values of parameters/arguments in the variable map
			}

			for (int x = 0; x < functionMap[functionName].getDefinition().size(); x++) {
				evaluateFunction(functionMap[functionName].getDefinition()[x], outputFile, functionName);
			}
		}
		break;

	case(DOC_WRITE) :
		if (lineFromFile[lineFromFile.find_first_of('(') +1] == '"') {
			for (int x = lineFromFile.find_first_of('(') + 2; lineFromFile[x] != '"'; x++) {
				toPrint += lineFromFile[x];
			}
			outputFile << toPrint;
		}
		else {
			int x = 15;
			while (lineFromFile[x] != ')') {
				toPrint += lineFromFile[x];
				x++;
			}
			outputFile << variableMap[toPrint];
		}

		//std::cout << "DONE WITH THIS LINE" << endl;
		break;

	case(FUNCTION_DEF) :
		space = lineFromFile.find_first_of(' ');
		leftParenth = lineFromFile.find_first_of('(');
		functionName = "";

		for (int x = space + 1; lineFromFile[x] != '('; x++) {
			functionName += lineFromFile[x];
		}
		//std::cout << "THE FUNCTION NAME IS: " << functionName << endl;

		for (int x = leftParenth + 1; lineFromFile[x] != ')'; x++) {
			parameterList += lineFromFile[x];
		}

		uf.setParameters(tokenize(parameterList, ","));

		std::getline(inputFile, lineFromFile);
		while (getLineType(lineFromFile) != END_BLOCK) {
			definition.push_back(lineFromFile.substr(lineFromFile.find_first_not_of(" ")));
			std::getline(inputFile, lineFromFile);
		}

		uf.setDefinition(definition);

		functionMap[functionName] = uf;
		definition.clear();
		break;

	case(RETURN) :
		break;

	case(END_BLOCK) :
		//do nothing
		break;

	case(IF) :
		space = lineFromFile.find_first_of(' ');
		leftParenth = lineFromFile.find_first_of('(');

		for (int x = leftParenth + 1; lineFromFile[x] != ')'; x++) {
			conditionalExpression += lineFromFile[x];
		}
		cout << "CONDITIONAL EXPRESSION: " << conditionalExpression << endl;

		std::getline(inputFile, lineFromFile);
		while (getLineType(lineFromFile) != END_BLOCK && getLineType(lineFromFile) != ELSE) {
			conditionalDefinition.push_back(lineFromFile.substr(lineFromFile.find_first_not_of(" ")));
			std::getline(inputFile, lineFromFile);
		}

		evalElse = !evaluateConditional(conditionalExpression);

		if (!evalElse) {
			for (int x = 0; x < conditionalDefinition.size(); x++) {
				evaluateFunction(conditionalDefinition[x], outputFile, functionName);
			}
		}

		conditionalExpression.clear();
		conditionalDefinition.clear();
		break;

	case(ELSE_IF) :
		break;

	case(ELSE) :
		break;
	}
}

double Interpreter::evaluateFunction(string s, ofstream& outputFile, string functionName) {
	string line = s, fn = functionName;
	toPrint = "";
	d = line.find_first_not_of(' ');
	line = line.substr(d);
	line = correctSpacing(line);
	tokenizedLine1 = tokenize(line, " ");
	std::cout << "Part of a method definition:  " << line << " | The line type is: " << getLineType(line) << endl;

	switch (getLineType(line)) {
	case(BLANK_LINE) :
		//do nothing for blank line
		break;

	case(DEFINE_VAR) :
		if (tokenizedLine1.size() == 4) {
			variableMap[tokenizedLine1[1]] = stod(tokenizedLine1[3]);
		}
		else {
			splitLine1 = tokenize(line, "=");
			//std::cout << splitLine1[1] << endl;
			rightSide1 = splitLine1[1];
			if (rightSide1[0] = ' ') {
				rightSide1 = rightSide1.substr(1);
			}

			testName = rightSide1.substr(0, rightSide1.find_first_of('('));

			if (functionMap.find(testName) != functionMap.end()) {
			}
			else {
				variableMap[tokenizedLine1[1]] = computeInfix(rightSide1);
			}
		}
		break;

	case(USER_DEFINED) :
		if (line.find_first_of('=') != -1) {
			splitLine1 = tokenize(line, "=");
			rightSide1 = splitLine1[1];
			if (rightSide1[0] == ' ') { rightSide1 = rightSide1.substr(1); }
			variableMap[tokenizedLine1[0]] = computeInfix(rightSide1);
		}
		else if (line.find_first_of('(') != -1) {
			line = line.substr(0, line.find_first_of('('));
			for (int x = 0; x < functionMap[line].getDefinition().size(); x++) {
				evaluateFunction(functionMap[line].getDefinition()[x], outputFile, functionName);
			}
		}

		break;

	case(DOC_WRITE) :
		if (line[15] == '"') {
			for (int x = 16; x < line.length() - 2; x++) {
				toPrint += line[x];
			}
			outputFile << toPrint;
		}
		else {
			int x = 15;
			while (line[x] != ')') {
				toPrint += line[x];
				x++;
			}
			//std::cout << "TOPRINT: " << toPrint << endl;
			outputFile << variableMap[toPrint];
		}

		break;

	case(RETURN) :
		std::cout << "THE FUNCTION IS: " << fn << ", THE VARIABLE IS: " << tokenizedLine1[1] << ", ITS VALUE IS: " << variableMap[tokenizedLine1[1]] << endl;
		functionMap[fn].setReturnValue(variableMap[tokenizedLine1[1]]);
		break;
	}
}

bool Interpreter::evaluateConditional(string conditional) {
	if (conditional.find_first_of('<') == -1) {
		conditionalSymbol = conditional[conditional.find_first_of('>')];
	}
	else {
		conditionalSymbol = conditional[conditional.find_first_of('<')];
	}

	splitCondtional = tokenize(conditional, conditionalSymbol);

	if (conditionalSymbol == "<") {
		return variableMap[splitCondtional[0]] < variableMap[splitCondtional[1]];
	}
	else {
		return variableMap[splitCondtional[0]] > variableMap[splitCondtional[1]];
	}
}