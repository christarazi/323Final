#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <regex>
#include "partone.h"
#include "partthree.h"

using namespace std;

// Function to check if string is a valid variable name.
bool isIdentifier(string s)
{
	if (s.compare("integer") == 0 || s.compare("begin") == 0 ||
		s.compare("end") == 0 || s.compare("var") == 0 || s.compare("print") == 0)
	{
		return false;
	}
	if (s[0] == '_' || isalpha(s[0]))
	{
		int space = s.find(" ");
		if (space == -1) return true;
		else return false;
	}
	else return false;
}

// Function to push to stack, reversing the order of the string parameter.
void pushToStack(string cell, vector<string> & s)
{
	// Reverse 'cell'.
	stack<string> reversedString;
	stringstream ss(cell);
	string temp;
	while (ss >> temp) reversedString.push(temp);
	//cout << "Pushing: ";
 	while (!reversedString.empty())
 	{
 		string temp = reversedString.top(); reversedString.pop();
 		temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
 		s.push_back(temp);
 		//cout << temp << " ";
 	}
}

// Function to show the stack.
void showStack(vector<string> s)
{
	cout << endl;
	for (auto&& i : s) cout << i;
}

// Function to detect certain mandatory keywords. Takes vector by reference.
bool detectKeywords(vector<string> & lines)
{
	bool keywordsIntact = true;
	// Checking for program keyword.
	if (find(lines.begin(), lines.begin()+1, "program") == lines.begin()+1)
	{
		auto programUpper = find(lines.begin(), lines.begin()+1, "PROGRAM");
		
		if (programUpper == lines.end()) { cout << "program is missing\n"; keywordsIntact = false; }
		else
		{
			int index = lines[0].find("PROGRAM");
			if (index == -1)
			{
				cout << "program is missing\n";
				keywordsIntact = false;
			}
			else
			{
				string replaceStr = "program";
				lines[0].replace(index, replaceStr.length(), replaceStr);
			}
		}
	}

	// Checking for var keyword.
	if (find(lines.begin(), lines.end(), "var") == lines.end())
	{
		auto varUpper = find(lines.begin(), lines.end(), "VAR");
		
		if (varUpper == lines.end()) { cout << "var is missing\n"; keywordsIntact = false; }
		else
		{
			int d = distance(lines.begin(), varUpper);
			int index = lines[d].find("VAR", 0);
			string replaceStr = "var";
			lines[d].replace(index, replaceStr.length(), replaceStr);
		}
	}

	// Checking for begin keyword.
	if (find(lines.begin(), lines.end(), "begin") == lines.end())
	{
		auto beginUpper = find(lines.begin(), lines.end(), "BEGIN");
		
		if (beginUpper == lines.end()) { cout << "begin is missing\n"; keywordsIntact = false; }
		else
		{
			int d = distance(lines.begin(), beginUpper);
			int index = lines[d].find("BEGIN", 0);
			string replaceStr = "begin";
			lines[d].replace(index, replaceStr.length(), replaceStr);
		}
	}

	// Checking for end. keyword.
	if (find(lines.begin(), lines.end(), "end.") == lines.end())
	{
		auto endUpper = find(lines.begin(), lines.end(), "END.");
		
		if (endUpper == lines.end()) { cout << "end is missing\n"; keywordsIntact = false; }
		else
		{
			int d = distance(lines.begin(), endUpper);
			int index = lines[d].find("END.", 0);
			string replaceStr = "end.";
			lines[d].replace(index, replaceStr.length(), replaceStr);
		}
	}

	// Checking for integer keyword.
	bool integer = false;
	for(auto&& j : lines)
	{
		if (j.find("integer") != -1)
		{
			integer = true; break;
		}
	}
	if (!integer) { cout << "integer is missing\n"; keywordsIntact = false; }

	// Checking for parenthesis after print statement.
	int i = 0;
	vector<bool> leftParansMissing;
	vector<bool> rightParansMissing;
	for(auto&& j : lines)
	{
		if (j.find("print") != -1)
		{
			leftParansMissing.push_back(lines[i].find("(") == -1);
			rightParansMissing.push_back(lines[i].find(")") == -1);
		}
		i++;
	}
	for (auto&& elem : leftParansMissing) if (elem) { cout << "( is missing" << endl; keywordsIntact = false; }
	for (auto&& elem : rightParansMissing) if (elem) { cout << ") is missing" << endl; keywordsIntact = false; }

	// Check for the existence and spelling of the print statements.
	regex findPrintStatement("^(\\w*)[ ]?\\(([ ]?[']?[ ]?.+?[ ]?[']?)\\)");
	smatch match;
	vector<string> printLineStatements;
	for (auto&& i : lines)
	{
		// Puts all print statements into a separate vector.
	    regex_search(i, match, findPrintStatement);
	    if (match[0].length() != 0) printLineStatements.push_back(i);
	}
	for (auto&& i : printLineStatements)
	{
	    regex_search(i, match, findPrintStatement);
	    if (match[1].length() == 0) { cout << "print is missing" << endl; keywordsIntact = false; }
	    else if (match[1].compare("print") != 0) { cout << "print is misspelled" << endl; keywordsIntact = false; }
	}
	return keywordsIntact;
}

// Function to check the line after the "var" line in text file.
vector<string> ensureVarDeclarations(vector<string> lines, bool & intact)
{
	vector<string> variables;
	vector<string> totalLines;
	vector<string> lineToProcess;

	for (auto&& i : lines) if (i.find("integer") != -1) totalLines.push_back(i);

	for (auto&& i : totalLines)
	{
		stringstream ss(i);
		string temp;
	    while (ss >> temp) 
	    {
	    	lineToProcess.push_back(temp);						// Pushes each token.
	    	if (isIdentifier(temp)) variables.push_back(temp);	// Pushes variables for later use.
	    }

	    // Handling the var declarations line.
	    for (int index = 0; index < lineToProcess.size();)
	    {
	        if (isIdentifier(lineToProcess[index]) && lineToProcess[index+1] == ",") index += 2;
	        else if (isIdentifier(lineToProcess[index]) && lineToProcess[index+1] != ","
	        	&& lineToProcess[index+2].compare("integer") != 0) 
	        {
	        	cout << "comma is missing" << endl;
	        	index++;
	        	intact = false;
	        }
	        else if (isIdentifier(lineToProcess[index]) && isIdentifier(lineToProcess[index+1]))
	        {
	        	cout << "comma is missing" << endl;
	        	index++;
	        	intact = false;
	        }
	        else if (lineToProcess[index] == ",") 
	        { 
	        	cout << "too many commas, can't process rest of line" << endl; 
	        	intact = false;
	        	break;
	        }
	        else if (isIdentifier(lineToProcess[index]) && lineToProcess[index+1].compare("integer") == 0) 
	        {
	        	cout << "colon is missing" << endl;
	        	index += 2;
	        	intact = false;
	        }
	        else if (isIdentifier(lineToProcess[index]) && lineToProcess[index+1] != ":"
	        	&& lineToProcess[index+2].compare("integer") == 0)
	        {
	        	cout << "colon is missing" << endl;
	        	index += 2;
	        	intact = false;
	        }
	        else if(lineToProcess[index].compare("integer") == 0) 
	        {
	        	if (lineToProcess[index-1] != ":") 
	        	{
	        		cout << "colon is missing" << endl;
		        	index++;
		        	intact = false;
	        	}
	        	else break;
	        }
	        else index++;
	    }

		lineToProcess.clear();
	}

	// Sort and get only unique variables.
	sort(variables.begin(), variables.end());
	auto last = unique(variables.begin(), variables.end());
	variables.erase(last, variables.end());

	return variables;
}

// Function to check if semicolons are at the end of each line that should have one.
bool checkSemiColons(vector<string> lines)
{
	bool semiColonsAfterLine = true;

	for (auto&& i : lines)
	{
		// Lines with begin, var, and end. don't need semicolons.
		if (i.find("begin") != -1 || i.find("var") != -1 || i.find("end.") != -1) continue;
	    int len = i.length();
	    if (i[len-1] != ';') { cout << "semicolon is missing" << endl; semiColonsAfterLine = false; }
	}

	return semiColonsAfterLine;
}

// Function that finds every variable used in assignments or print statements.
vector<string> findVariablesUsed(vector<string> lines)
{
	vector<string> variables;

	for (auto&& i : lines)
	{
		if (i.find("=") != -1 || i.find("print") != -1) 
		{
			stringstream ss(i);
			string temp;
			while (ss >> temp)
			{
				// Handles quotes inside the print statement. We don't want what's inside quotes.
				if (temp.find("'") != -1)
				{
					ss >> temp;
					while (temp.find("'") == -1) ss >> temp;
					ss >> temp;
				}
				if (isIdentifier(temp)) variables.push_back(temp);
			}
		}
	}

	// Sort and get only unique variables.
	sort(variables.begin(), variables.end());
	auto last = unique(variables.begin(), variables.end());
	variables.erase(last, variables.end());

	return variables;
}

// Function show the differences between declared and used variables.
void checkVariables(vector<string> variables, vector<string> usedVars)
{
	vector<string> differences;

	// Examines usedVars vector; if usedVars has anything that variables does not, insert in differences.
	set_difference(usedVars.begin(), usedVars.end(), variables.begin(), variables.end(), 
		inserter(differences, differences.begin()));

	for (auto&& i : differences) cout << "unidentified variable " + i << endl;
}

// Function run the LL(1) parsing.
bool parsingTable(deque<string> inputQueue)
{
	/* 
	Initializing two dimensional map with the grammar.
	'table' is a map of a map of strings.
	//*/
	map<string, map<string, string>> table
	{
		{"P",
		{
			{"program", "program I ; var D begin S end."}
		}},

		{"I",
		{
			{"a", "I' U"}, {"b", "I' U"}, {"c", "I' U"}, {"d", "I' U"}, {"e", "I' U"}
		}},

		{"D",
		{
			{"a", "D' : T ;"}, {"b", "D' : T ;"}, {"c", "D' : T ;"}, {"d", "D' : T ;"}, {"e", "D' : T ;"}
		}},

		{"D'",
		{
			{"a", "I Z"}, {"b", "I Z"}, {"c", "I Z"}, {"d", "I Z"}, {"e", "I Z"}
		}},

		{"Z",
		{
			{",", ", D'"}, {":", "λ"}
		}},

		{"T",
		{
			{"integer", "integer"}, {":", "λ"}
		}},

		{"S'",
		{
			{"print", "P'"}, {"a", "A"}, {"b", "A"}, {"c", "A"}, {"d", "A"}, {"e", "A"}
		}},

		{"P'",
		{
			{"print", "print ( <string> , I ) ;"}, {":", "λ"}
		}},

		{"A",
		{
			{"a", "I = E ;"}, {"b", "I = E ;"}, {"c", "I = E ;"}, {"d", "I = E ;"}, {"e", "I = E ;"}
		}},

		{"E",
		{
			{"a", "T' R"}, {"b", "T' R"}, {"c", "T' R"}, {"d", "T' R"}, {"e", "T' R"}, {"+", "T' R"}, {"-", "T' R"}, {"(", "T' R"},
			{"0", "T' R"}, {"1", "T' R"}, {"2", "T' R"}, {"3", "T' R"}, {"4", "T' R"}, {"5", "T' R"}, {"6", "T' R"}, {"7", "T' R"}, 
			{"8", "T' R"}, {"9", "T' R"}
		}},

		{"R",
		{
			{"+", "+ T' R"}, {"-", "- T' R"}, {";", "λ"}, {")", "λ"}
		}},

		{"T'",
		{
			{"a", "F X"}, {"b", "F X"}, {"c", "F X"}, {"d", "F X"}, {"e", "F X"}, {"(", "F X"},
			{"0", "F X"}, {"1", "F X"}, {"2", "F X"}, {"3", "F X"}, {"4", "F X"}, {"5", "F X"}, {"6", "F X"}, {"7", "F X"}, 
			{"8", "F X"}, {"9", "F X"}
		}},

		{"X",
		{
			{"*", "* F X"}, {"/", "/ F X"}, {";", "λ"}, {")", "λ"}, {"+", "λ"}, {"-", "λ"}
		}},

		{"F",
		{
			{"a", "I"}, {"b", "I"}, {"c", "I"}, {"d", "I"}, {"e", "I"}, {"+", "N"}, {"-", "N"}, {"(", "( E )"},
			{"0", "N"}, {"1", "N"}, {"2", "N"}, {"3", "N"}, {"4", "N"}, {"5", "N"}, {"6", "N"}, {"7", "N"}, 
			{"8", "N"}, {"9", "N"}
		}},

		{"N",
		{
			{"+", "S'' D'' U'"}, {"-", "S'' D'' U'"}, {"0", "D'' U'"}, {"1", "D'' U'"}, {"2", "D'' U'"}, {"3", "D'' U'"}, {"4", "D'' U'"}, {"5", "D'' U'"}, {"6", "D'' U'"}, {"7", "D'' U'"}, 
			{"8", "D'' U'"}, {"9", "D'' U'"}
		}},

		{"U'",
		{
			{";", "λ"}, {")", "λ"}, {"+", "λ"}, {"-", "λ"}, {"*", "λ"}, {"/", "λ"}, {",", "λ"}, {":", "λ"}, {"=", "λ"}, 
			{"0", "D'' U'"}, {"1", "D'' U'"}, {"2", "D'' U'"}, {"3", "D'' U'"}, {"4", "D'' U'"}, {"5", "D'' U'"}, {"6", "D'' U'"}, 
			{"7", "D'' U'"}, {"8", "D'' U'"}, {"9", "D'' U'"}
		}},

		{"S''",
		{
			{"+", "+"}, {"-", "-"}, {";", "λ"}, {")", "λ"}
		}},

		{"D''",
		{
			{"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, 
			{"8", "8"}, {"9", "9"}
		}},

		{"U",
		{
			{"=", "λ"}, {",", "λ"}, {";", "λ"}, {":", "λ"}, {")", "λ"}, {"/", "λ"}, {"*", "λ"}, {"+", "λ"}, {"-", "λ"},
			{"a", "I' U"}, {"b", "I' U"}, {"c", "I' U"}, {"d", "I' U"}, {"e", "I' U"},
			{"0", "D'' U"}, {"1", "D'' U"}, {"2", "D'' U"}, {"3", "D'' U"}, {"4", "D'' U"}, {"5", "D'' U"}, {"6", "D'' U"}, {"7", "D'' U"}, 
			{"8", "D'' U"}, {"9", "D'' U"}
		}},

		{"I'",
		{
			{"a", "a"}, {"b", "b"}, {"c", "c"}, {"d", "d"}, {"e", "e"}
		}},

		{"S",
		{
			{"print", "S' S"}, {"a", "S' S"}, {"b", "S' S"}, {"c", "S' S"}, {"d", "S' S"}, {"e", "S' S"}, {"end.", "λ"}
		}}
	};

	// Boolean variable to check if word is accepted. Used at the end of the program.
 	bool accepted = true;

 	// Initialize the stack with $ then P (starting point of grammar).
 	vector<string> stateStack;
	stateStack.push_back("$");
	stateStack.push_back("P");

	/* 
	current holds the current token from the input.
	topOfStack holds the token from the stateStack.
	cell holds the data which we retrieve from the table.
	//*/
	string current; string topOfStack; string cell;

	string firstSingleQuote = "'";
	string secondSingleQuote = "'";

	current = inputQueue.front();
	inputQueue.pop_front();

	top:
	while(inputQueue.size() != 0)
	{
		// Handles quotes inside the print statement.
		if (current.compare(firstSingleQuote) == 0)
		{
			current = inputQueue.front();
			inputQueue.pop_front();
			while (current.compare(firstSingleQuote) != 0)
			{
				current = inputQueue.front();
				inputQueue.pop_front();
			}
			current = inputQueue.front();
			inputQueue.pop_front();
		}

		topOfStack = stateStack.back();
		stateStack.pop_back();

		if (topOfStack.compare("<string>") == 0) continue;

		while (current.compare(topOfStack) == 0)
		{
			match:
			//cout << "Matched first! - " << current << " " << topOfStack << endl;
			current = inputQueue.front();
			inputQueue.pop_front();

			topOfStack = stateStack.back();
			stateStack.pop_back();

			if (topOfStack.compare("<string>") == 0) goto end;
			if (current.compare("$") == 0 && topOfStack.compare("$") == 0) goto accepted;
		}
		
		cell = table[topOfStack][current];
		//cout << "Getting - table[" << topOfStack + "][" + current + "] = " + cell << endl;

		if (cell.length() == 0)
		{
			cout << endl << "ERROR" << endl; accepted = false;
			break;
		}
		else if (cell.compare("λ") == 0)
		{
			while (cell.compare("λ") == 0)
			{
				//cout << "We found lamda!" << endl;
				topOfStack = stateStack.back();
				stateStack.pop_back();

				if (topOfStack.compare("<string>") == 0) goto end;
				
				cell = table[topOfStack][current];
				//cout << "Getting - table[" << topOfStack + "][" + current + "] = " + cell << endl;
				if (cell.compare("λ") != 0 && cell.length() != 0) 
				{
					pushToStack(cell, stateStack);
					showStack(stateStack);
				}
				if (current.compare(topOfStack) == 0)
				{
					//cout << "Going to match!" << endl;
					goto match;
				}
			}
		}
		else
		{
			pushToStack(cell, stateStack);
			showStack(stateStack);
		}
		end:
		//cout << "Restarting!" << endl;
		goto top;

		accepted:
		cout << endl << "Accepted!" << endl;
		accepted = true; break;
	}

	// If 'accepted' is true, program is accepted.
	if (accepted) { cout << endl << "Program accepted." << endl; return true; }
	else 		  { cout << endl << "Program denied." << endl; return false; }
}

int main()
{
	initiateParsing();	// Call function to initiate part one.

	bool startPartThree, keywordsIntact, semiColons;
	bool varDeclarationsIntact = true;
	fstream fIn("finalp2.txt", ios::in);
	string line;

	// Reserved word list; this is needed so we can differentiate between terminals and non-terminals.
	vector<string> reserved = {"program", "begin", "var", "end.", "integer", "print"};
	vector<string> variablesDeclared;
	vector<string> variablesUsed;

	deque<string> inputFromTextFile;
	vector<string> lines;

	// Get every line in the text file and insert the whole line as a string.
	while(getline(fIn, line)) lines.push_back(line);
	fIn.close();

	keywordsIntact = detectKeywords(lines);
	semiColons = checkSemiColons(lines);

	if (keywordsIntact && semiColons)
	{
		// Call to this changes varDeclarationsIntact.
		variablesDeclared = ensureVarDeclarations(lines, varDeclarationsIntact);	

		if (varDeclarationsIntact)
		{
			variablesUsed = findVariablesUsed(lines);
			checkVariables(variablesDeclared, variablesUsed);

			// Insert reserved words whole and non-reserved words character by character. 
			for(auto&& i : lines)
			{
				stringstream ss(i);
				string temp;
				while(ss >> temp)
				{
					if (find(reserved.begin(), reserved.end(), temp) != reserved.end()) inputFromTextFile.push_back(temp);
					else for(auto&& k : temp) inputFromTextFile.push_back(string(1,k));
				}
			}
			inputFromTextFile.push_back("$");

			startPartThree = parsingTable(inputFromTextFile);	// Begin parsing using LL(1).

			if (startPartThree) initiateConversion();	// Call to function to initiate part three.
		}
	}

	return 0;
}