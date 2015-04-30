/* So far this works for the input file "finalp2.txt" and detecting the keywords: "program", "begin" and "end.". */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
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
	// Iterates backwards to push 'cell' in reverse order.
 	for (int i = cell.size()-1; i >= 0; --i)
 	{
 		// Using string constructor here because cell[i] is of char type.
 		s.push_back(string(1, cell[i]));
 	}
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


// Function to detect certain mandatory keywords.
void detectKeywords(vector<string> l)
{
	string temp = "";
	for (auto&& i : l.front())
	{
		temp += tolower(i);
	}
	// string.find() function searches within the substring.
	int programKeyword = temp.find("program");

	if (programKeyword == -1)
	{
		cout << "program is expected \n";
	}

	// seems like the find() function searches for an exact match, rather than searching the substring.
	
	//bool programWord = (find(begin(l), end(l), "program") != l.end());	
	bool beginWord = (find(begin(l), end(l), "begin") != l.end());
	bool varWord = (find(begin(l), end(l), "var") != l.end());
	bool endWord = (find(begin(l), end(l), "end.") != l.end());

	//if (!programWord) cout << "program is expected \n";
	if (!beginWord) cout << "begin is expected \n";
	if (!varWord) cout << "var is expected \n";
	if (!endWord) cout << "end. is expected \n";
}

// Function to check the line after the "var" line in text file.
void ensureVarDeclarations(vector<string> l)
{
	int index = 0;
	for (auto&& i : l)
	{
		index++;
		if (i.compare("var") == 0)
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

void parsingTable(vector<string> l)
{
	// Initializing two dimensional map with the grammar.
	// 'table' is a map of a map of strings.
	map<string, map<string, string>> table
	{
		{"P",
		{
			{"program", "program I; var D begin S end."}
		}},

		{"I",
		{
			{"a", "I'U"}, {"b", "I'U"}, {"c", "I'U"}, {"d", "I'U"}, {"e", "I'U"}
		}},

		{"D",
		{
			{"a", "D':T;"}, {"b", "D':T;"}, {"c", "D':T;"}, {"d", "D':T;"}, {"e", "D':T;"}
		}},

		{"D'",
		{
			{"a", "IZ"}, {"b", "IZ"}, {"c", "IZ"}, {"d", "IZ"}, {"e", "IZ"}
		}},

		{"Z",
		{
			{",", ",D"}, {":", "λ"}
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
			{"print", "print( <string> ,I);"}, {":", "λ"}
		}},

		{"A",
		{
			{"a", "I=E;"}, {"b", "I=E;"}, {"c", "I=E;"}, {"d", "I=E;"}, {"e", "I=E;"}
		}},

		{"E",
		{
			{"a", "T'R"}, {"b", "T'R"}, {"c", "T'R"}, {"d", "T'R"}, {"e", "T'R"}, {"+", "T'R"}, {"-", "T'R"}, {"(", "T'R"},
			{"0", "T'R"}, {"1", "T'R"}, {"2", "T'R"}, {"3", "T'R"}, {"4", "T'R"}, {"5", "T'R"}, {"6", "T'R"}, {"7", "T'R"}, 
			{"8", "T'R"}, {"9", "T'R"}
		}},

		{"R",
		{
			{"+", "+T'R"}, {"-", "+T'R"}, {";", "λ"}, {")", "λ"}, {"e", "I=E;"}
		}},

		{"T'",
		{
			{"a", "FX"}, {"b", "FX"}, {"c", "FX"}, {"d", "FX"}, {"e", "FX"}, {"(", "FX"},
			{"0", "FX"}, {"1", "FX"}, {"2", "FX"}, {"3", "FX"}, {"4", "FX"}, {"5", "FX"}, {"6", "FX"}, {"7", "FX"}, 
			{"8", "FX"}, {"9", "FX"}
		}},

		{"X",
		{
			{"*", "*FX"}, {"/", "/FX"}, {";", "λ"}, {")", "λ"}, {"e", "I=E;"}
		}},

		{"F",
		{
			{"a", "I"}, {"b", "I"}, {"c", "I"}, {"d", "I"}, {"e", "I"}, {"+", "N"}, {"-", "N"}, {"(", "(E)"},
			{"0", "N"}, {"1", "N"}, {"2", "N"}, {"3", "N"}, {"4", "N"}, {"5", "N"}, {"6", "N"}, {"7", "N"}, 
			{"8", "N"}, {"9", "N"}
		}},

		{"N",
		{
			{"+", "S''D''U'"}
		}},

		{"U'",
		{
			{";", "λ"}, {")", "λ"}, {"0", "D''U'"}, {"1", "D''U'"}, {"2", "D''U'"}, {"3", "D''U'"}, {"4", "D''U'"}, {"5", "D''U'"}, {"6", "D''U'"}, 
			{"7", "D''U'"}, {"8", "D''U'"}, {"9", "D''U'"}
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
			{"=", "λ"}, {",", "λ"}, {";", "λ"}, {":", "λ"}, {")", "λ"}
		}}
	};

	// Boolean variable to check if word is accepted. Used at the end of the program.
 	bool accepted = true;
 	
	for (auto&& i : l)
	{
		vector<string> s;
		string word = i;

		// Initialize the stack with $ then E (starting point of grammar).
		s.push_back("$");
		s.push_back("P");

	 	// 'current' will be holding the current char. in 'word'.
		string current;
		int counter = 0;

		// While the stack is not empty...
		while(!s.empty())
		{
			//...set 'current' to a char indexed by 'counter'.
			current = word[counter];
	 
	 		// If top of stack matches 'current'...
			if(s.back().compare(current) == 0)
			{
				// ...we have found a match...output stack.
				// Increment counter, pop off the stack and ignore the rest of the iteration.
				showStack(s);
				counter++;
				s.pop_back();
				continue;
			}

			// If top of stack does not match 'current'...
			else
			{
				// ...'temp' holds top of stack; pop off stack.
				string temp = s.back();
				s.pop_back();
	 			
	 			// Get value from 'table' indexed by 'temp' and 'current'.
	 			string cell = table[temp][current];

	 			// If the value returned from the table is nothing, we can reject the word.
	 			if (cell.size() == 0)
	 			{
	 				accepted = false;
	 				break;
	 			}

	 			// If the value returned from the table is lambda, we need to enter a while loop so we can keep popping.
	 			if (cell.compare("λ") == 0)
	 			{
	 				// While we are still seeing lambda from the table...
	 				while (cell.compare("λ") == 0)
		 			{
		 				// ...pop off the stack.
		 				temp = s.back();
		 				s.pop_back();

		 				// If stack returns a "$" and the current char. is also a "$"...
		 				if (temp.compare("$") == 0 && current.compare("$") == 0)
		 				{
		 					// ...we have reached the end. Word is accepted.
		 					break;
		 				}

		 				// If stack returns the same thing that 'current' is...
		 				if (temp.compare(current) == 0)
		 				{
		 					// ...we found a match; show stack and increment counter.
		 					showStack(s);
		 					counter++;
		 				}

		 				// Get value from 'table' located at 'temp' and 'current'.
		 				cell = table[temp][current];

		 				// If that value is NOT lambda...
	 					if (cell.compare("λ") != 0)
	 					{
	 						// ...we need to push the contents of 'cell' to stack.
	 						pushToStack(cell, s);
	 					}
	 				}
	 			}

	 			// If the value returned from the table is anything but lambda...
	 			else
	 			{
	 				// ...need to push to stack.
	 				pushToStack(cell, s);
	 			}
			}
		}
	}

	// If 'accepted' is true, word is accepted.
	if (accepted) cout << "Word accepted." << endl;
	else 		  cout << "Word denied." << endl;
}

int main()
{
	vector<string> l;
	fstream fIn("finalp2.txt", ios::in);
	string line;

	while(getline(fIn, line))
	{
		l.push_back(line);
	}
	fIn.close();

	detectKeywords(l);
	ensureVarDeclarations(l);
	parsingTable(l);

	return 0;
}