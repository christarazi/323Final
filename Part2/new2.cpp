/* So far this works for the input file "finalp2.txt" and detecting the keywords: "program", "begin" and "end.". */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

// Function to check if string is a valid variable name.
bool isIdentifier(string s)
{
	if (s.compare("integer") == 0 || s.compare("begin") == 0 ||
		s.compare("end") == 0 || s.compare("var") == 0)
	{
		return false;
	}
	if (s[0] == '_' || isalpha(s[0]))
	{
		int space = s.find(" ");
		if (space == -1)
		{
			return true;
		}
		else return false;
	}
	else return false;
}

// Function to push to stack.
void pushToStack(string cell, vector<string> & s)
{
	// Reverse 'cell'.
	stringstream ss(cell);
	stack<string> reversedString;
	string temp;
	while (ss >> temp)
	{
		reversedString.push(temp);
	}
	cout << "Pushing: ";
 	while (!reversedString.empty())
 	{
 		string temp = reversedString.top(); reversedString.pop();
 		temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
 		s.push_back(temp);
 		cout << temp << " ";
 	}
 	cout << "\n";
}

// Function to show the stack.
void showStack(vector<string> s)
{
	cout << endl;
	for (auto&& i : s)
	{
		cout << i;
	}
	cout << endl;
}


// Function to detect certain mandatory keywords. Takes vector by reference.
void detectKeywords(vector<string> & l)
{
	bool end, begin, program, var;
	end = begin = program = var = true;
	
	if (find(l.begin(), l.end(), "program") == l.end())
	{
		auto programUpper = find(l.begin(), l.begin()+1, "PROGRAM");
		
		if (programUpper == l.end()) cout << "program is missing\n";
		else
		{
			int index = l[0].find("PROGRAM", 0);
			string replaceStr = "program";
			l[0].replace(index, replaceStr.length(), replaceStr);
		}
	}

	if (find(l.begin(), l.end(), "var") == l.end())
	{
		auto varUpper = find(l.begin(), l.end(), "VAR");
		
		if (varUpper == l.end()) cout << "var is missing\n";
		else
		{
			int d = distance(l.begin(), varUpper);
			int index = l[d].find("VAR", 0);
			string replaceStr = "var";
			l[d].replace(index, replaceStr.length(), replaceStr);
		}
	}

	if (find(l.begin(), l.end(), "begin") == l.end())
	{
		auto beginUpper = find(l.begin(), l.end(), "BEGIN");
		
		if (beginUpper == l.end()) cout << "begin is missing\n";
		else
		{
			int d = distance(l.begin(), beginUpper);
			int index = l[d].find("BEGIN", 0);
			string replaceStr = "begin";
			l[d].replace(index, replaceStr.length(), replaceStr);
		}
	}

	if (find(l.begin(), l.end(), "end.") == l.end())
	{
		auto endUpper = find(l.begin(), l.end(), "END.");
		
		if (endUpper == l.end()) cout << "end is missing\n";
		else
		{
			int d = distance(l.begin(), endUpper);
			int index = l[d].find("END.", 0);
			string replaceStr = "end.";
			l[d].replace(index, replaceStr.length(), replaceStr);
		}
	}
}

// Function to check the line after the "var" line in text file.
void ensureVarDeclarations(vector<string> l)
{
	int index = 0;
	for (auto&& i : l)
	{
		index++;
		if (i.compare("var") == 0 || i.compare("VAR") == 0)
		{
			break;
		}
	}

	string temp = l[index];
	int integerKeyword = temp.find("integer");

	stringstream ss(temp);	// tokenize temp string
	string token;
	vector<string> v;
	while(ss >> token) v.push_back(token);	// insert each token in a vector

	if (integerKeyword != -1)
	{
		int IDs = 0;
		int commas = 0;
		int colons = 0;
		for (int i = 0; i < v.size(); i++)
		{
			if (isIdentifier(v[i]))
			{
				IDs++;
			}
			else if (v[i] == ",")
			{
				commas++;
			}
			else if (v[i] == ":")
			{
				colons++;
			}
		}

		if (IDs - commas > 1) cout << "more than one comma is missing\n";
		else if (IDs - commas < 1 && (IDs - commas) != 1) cout << "too many commas\n";

		if (colons != 1) cout << "colon is missing\n";

		if (v[v.size()-1] != ";")
		{
			cout << "semicolon is missing\n";
		}
	}	
}

void parsingTable(deque<string> inputQueue)
{
	// Initializing two dimensional map with the grammar.
	// 'table' is a map of a map of strings.
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
			{"print", "print( <string> , I) ;"}, {":", "λ"}
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
			{"+", "+ T' R"}, {"-", "+ T' R"}, {";", "λ"}, {")", "λ"}, {"e", "I = E ;"}
		}},

		{"T'",
		{
			{"a", "F X"}, {"b", "F X"}, {"c", "F X"}, {"d", "F X"}, {"e", "F X"}, {"(", "F X"},
			{"0", "F X"}, {"1", "F X"}, {"2", "F X"}, {"3", "F X"}, {"4", "F X"}, {"5", "F X"}, {"6", "F X"}, {"7", "F X"}, 
			{"8", "F X"}, {"9", "F X"}
		}},

		{"X",
		{
			{"*", "* F X"}, {"/", "/ F X"}, {";", "λ"}, {")", "λ"}, {"e", "I = E ;"}
		}},

		{"F",
		{
			{"a", "I"}, {"b", "I"}, {"c", "I"}, {"d", "I"}, {"e", "I"}, {"+", "N"}, {"-", "N"}, {"(", "( E )"},
			{"0", "N"}, {"1", "N"}, {"2", "N"}, {"3", "N"}, {"4", "N"}, {"5", "N"}, {"6", "N"}, {"7", "N"}, 
			{"8", "N"}, {"9", "N"}
		}},

		{"N",
		{
			{"+", "S'' D'' U'"}
		}},

		{"U'",
		{
			{";", "λ"}, {")", "λ"}, {"0", "D'' U'"}, {"1", "D'' U'"}, {"2", "D'' U'"}, {"3", "D'' U'"}, {"4", "D'' U'"}, {"5", "D'' U'"}, {"6", "D'' U'"}, 
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
			{"=", "λ"}, {",", "λ"}, {";", "λ"}, {":", "λ"}, {")", "λ"},
			{"a", "I' U"}, {"b", "I' U"}, {"c", "I' U"}, {"d", "I' U"}, {"e", "I' U"},
			{"0", "D'' U"}, {"1", "D'' U"}, {"2", "D'' U"}, {"3", "D'' U"}, {"4", "D'' U"}, {"5", "D'' U"}, {"6", "D'' U"}, {"7", "D'' U"}, 
			{"8", "D'' U"}, {"9", "D'' U"}
		}},

		{"I'",
		{
			{"a", "a"}, {"b", "b"}, {"c", "c"}, {"d", "d"}, {"e", "e"}
		}}
	};

	// Boolean variable to check if word is accepted. Used at the end of the program.
 	bool accepted = true;

 	// Initialize the stack with $ then P (starting point of grammar).
 	vector<string> stateStack;
	stateStack.push_back("$");
	stateStack.push_back("P");
	// current holds the current token from the input.
	// topOfStack holds the token from the stateStack.
	// cell holds the data which we retrieve from the table.
	string current; string topOfStack; string cell;

	current = inputQueue.front();
	inputQueue.pop_front();

	while(inputQueue.size() != 0)
	{
		topOfStack = stateStack.back();
		stateStack.pop_back();

		while (current.compare(topOfStack) == 0)
		{
			match:
			cout << "Matched! - " << current << " " << topOfStack << endl;
			current = inputQueue.front();
			inputQueue.pop_front();

			topOfStack = stateStack.back();
			stateStack.pop_back();
		}
		
		cell = table[topOfStack][current];
		cout << "Getting - table[" << topOfStack + "][" + current + "] = " + cell << endl;

		if (cell.length() == 0)
		{
			cout << "ERROR" << endl;
			break;
		}
		else if (cell.compare("λ") == 0)
		{
			while (cell.compare("λ") == 0)
			{
				cout << "We found lamda!" << endl;
				topOfStack = stateStack.back();
				stateStack.pop_back();
				cell = table[topOfStack][current];
				cout << "Getting - table[" << topOfStack + "][" + current + "] = " + cell << endl;
				if (cell.compare("λ") != 0 && cell.length() != 0) 
				{
					pushToStack(cell, stateStack);
					showStack(stateStack);
				}
				if (current.compare(topOfStack) == 0)
				{
					cout << "Going to match!" << endl;
					goto match;
				}
			}
		}
		else
		{
			pushToStack(cell, stateStack);
			showStack(stateStack);
		}
	}

	// If 'accepted' is true, word is accepted.
	if (accepted) cout << "Word accepted." << endl;
	else 		  cout << "Word denied." << endl;
}

int main()
{
	deque<string> queue;
	vector<string> l;
	fstream fIn("finalp2.txt", ios::in);
	string line;

	// Reserved word list; this is needed so we can differentiate between terminals and non-terminals.
	vector<string> reserved = {"program", "begin", "var", "end.", "integer", "print"};

	while(getline(fIn, line))
	{
		l.push_back(line);		// For other parts of program.
	}
	fIn.close();

	detectKeywords(l);
	ensureVarDeclarations(l);

	for(auto&& i : l)
	{
		stringstream ss(i);
		string temp;
		while(ss >> temp)
		{
			if (find(reserved.begin(), reserved.end(), temp) != reserved.end())
			{
				queue.push_back(temp);
			}
			else
			{
				for(auto&& k : temp)
				{
					queue.push_back(string(1,k));
				}
			}
		}
	}

	
	parsingTable(queue);
	

	return 0;
}