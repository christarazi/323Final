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
	//cout << "Pushing: ";
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

void parsingTable(vector<string> l)
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
			{"a", "D' : T ;"}, {"b", "D' : T ;"}, {"c", "D' :T ;"}, {"d", "D' : T ;"}, {"e", "D' : T ;"}
		}},

		{"D'",
		{
			{"a", "I Z"}, {"b", "I Z"}, {"c", "I Z"}, {"d", "I Z"}, {"e", "I Z"}
		}},

		{"Z",
		{
			{",", ", D"}, {":", "λ"}
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

 	// Initialize the stack with $ then E (starting point of grammar).
 	vector<string> s;
	s.push_back("$");
	s.push_back("P");

	// Reserved word list; this is needed so we can differentiate between terminals and non-terminals.
	vector<string> reserved = {"program", "begin", "var", "end.", "integer", "print"};

	stringstream ss;
	for (auto&& i : l)
	{
		ss << i;
	}

	string tmep;
	while(ss >> tmep)
	{
		cout << tmep << "\n";
	}

	/*
	for (auto&& i : l)
	{
		string word = i;

	 	// 'current' will be holding the current char. in 'word'.
		string current;
		string orignalWord;
		stringstream ss(word);
		cout << "NEW WORD: " << word << endl;
		int counter = 0;
		ss >> current;
		bool useReserved = false;

		// While the stack is not empty...
		while(!s.empty())
		{
			cout << "\n\n\n";
			if (find(reserved.begin(), reserved.end(), current) != reserved.end()) 
			{ 
				cout << "using useReserved\n"; 
				useReserved = true;
			}
			else 
			{
				cout << "using counter\n"; 
				current = orignalWord[counter]; 
				useReserved = false;
				cout << "CURRENT LENGTH = " << current.length() << endl;
				if (counter >= orignalWord.length())
				{
					cout << "IT HAPPENS\n"; 
					ss >> current; 
					orignalWord = current; 
					counter = 0;
					useReserved = true;
				}
			}

			//...set 'current' to a char indexed by 'counter'.
			cout << "current: " << current << endl;
	 
	 		// If top of stack matches 'current'...
			if(s.back().compare(current) == 0)
			{
				// ...we have found a match...output stack.
				// Increment counter, pop off the stack and ignore the rest of the iteration.
				showStack(s);
				//counter++;
				if (useReserved) { ss >> current; orignalWord = current; counter = 0; }
				else counter++;
				s.pop_back();
				cout << "Found a match!\n";

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
	 			cout << "CELL table[" << temp << "][" << current << "] = " << cell << "\n" ;

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
		 					//counter++;
		 					if (useReserved) { ss >> current; orignalWord = current; counter = 0; }
		 					else counter++;
		 					cout << "2 Found a match!\n";
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
	*/

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