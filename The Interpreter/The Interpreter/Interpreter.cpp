#include "Interpreter.h"
#include <stack>

string lineFromFile, temp = "", rightSide = "", left = "";
int lineNumber = 0, space, leftParenth, d;
vector<string> tokenizedLine, splitLine, definition;
string variableName, toPrint, functionName, parameterList;
double variableValue;

void Interpreter::interpretScript(ifstream& inputFile, ofstream& outputFile) {
	while (getline(inputFile, lineFromFile)) {
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

	/*cout << infixExpression << endl;
	for (int x = 0; x < postfixExpression.size(); x++) {
	cout << postfixExpression[x] << " ";
	}
	cout << endl;*/

	for (int x = 0; x < postfixExpression.size(); x++) {
		if (getOperatorType(postfixExpression[x]) == NOT_OPERATOR) {
			if (getCharType(postfixExpression[x]) == DIGIT) {
				calcStack.push(stod(postfixExpression[x]));
				//cout << "DIGIT: " << postfixExpression[x] << endl;
			}
			else if (getCharType(postfixExpression[x]) == VARIABLE) {
				calcStack.push(variableMap[postfixExpression[x]]);
				//cout << "VARIABLE: " << postfixExpression[x] << " = " << variableMap[postfixExpression[x]] <<  endl;
			}
		}

		else {
			temp1 = calcStack.top();
			calcStack.pop();
			temp2 = calcStack.top();
			calcStack.pop();
			//cout << temp1 << "|" << temp2 << endl;

			switch (getOperatorType((postfixExpression[x]))) {
			case (MULTIPLICATION) :
				calcStack.push(temp2 * temp1);
				//cout << "MULTIPLICATION" << endl;
				break;

			case(ADDITION) :
				calcStack.push(temp2 + temp1);
				//cout << "ADDITION" << endl;
				break;

			case(DIVISION) :
				calcStack.push(temp2 / temp1);
				//cout << "DIVISION" << endl;
				break;

			case(SUBTRACTION) :
				calcStack.push(temp2 - temp1);
				//cout << "SUBTRACTION" << endl;
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
		if (getOperatorType(s) == DIVISION) {
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
	lineNumber++;
	toPrint = "";
	functionName = "";
	parameterList = "";
	LINE_TYPE lineType = getLineType(lineFromFile); // Check Parser.h for the different line types
	cout << "line " << lineNumber << " is type: " << lineType << endl;
	cout << lineFromFile << " | ";
	lineFromFile = correctSpacing(lineFromFile);
	cout << lineFromFile << endl;
	tokenizedLine = tokenize(lineFromFile, " ");
	UserFunction uf;
	// Use your interpreter to execute each line

	switch (lineType) {
	case(BLANK_LINE) :
		//do nothing for blank line
		break;
	case(DEFINE_VAR) :
		if (tokenizedLine.size() == 4) {
			variableMap[tokenizedLine[1]] = stod(tokenizedLine[3]);
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

			cout << "Function being called: " << functionName << endl;
			//functionMap[functionName].call(outputFile);
			for (int x = 0; x < functionMap[functionName].getDefinition().size(); x++) {
				evaluateFunction(functionMap[functionName].getDefinition()[x], outputFile);
			}
		}
		break;

	case(DOC_WRITE) :
		if (lineFromFile[15] == '"') {
			for (int x = 16; x < lineFromFile.length() - 3; x++) {
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
			cout << toPrint << endl;
			outputFile << variableMap[toPrint];
		}

		break;

	case(FUNCTION_DEF) :
		space = lineFromFile.find_first_of(' ');
		leftParenth = lineFromFile.find_first_of('(');
		functionName = "";

		for (int x = space + 1; lineFromFile[x] != '('; x++) {
			functionName += lineFromFile[x];
		}
		cout << "THE FUNCTION NAME IS: " << functionName << endl;

		for (int x = leftParenth + 1; lineFromFile[x] != ')'; x++) {
			parameterList += lineFromFile[x];
		}
		cout << "PARAMETER LIST STRING: " << parameterList << endl;

		if (parameterList.find_first_of(',') == -1) {
			uf.setParameters(parameterList);
		}
		else {
			uf.setParameters(tokenize(parameterList, ","));
		}

		getline(inputFile, lineFromFile);
		while (getLineType(lineFromFile) != END_BLOCK) {
			definition.push_back(lineFromFile);
			getline(inputFile, lineFromFile);
		}

		uf.setDefinition(definition);

		functionMap[functionName] = uf;
		break;

	case(RETURN) :
		break;

	case(END_BLOCK) :
		//do nothing
		break;

	case(IF) :
		break;

	case(ELSE_IF) :
		break;

	case(ELSE) :
		break;
	}
}

double Interpreter::evaluateFunction(string s, ofstream& outputFile) {
	string line = s;
	toPrint = "";

	switch (getLineType(line)) {
	case(BLANK_LINE) :
		//do nothing for blank line
		break;

	case(DOC_WRITE) :
		d = line.find_first_of('d');
		line = line.substr(d);
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
			cout << toPrint << endl;
			outputFile << variableMap[toPrint];
		}

		break;
	}
}