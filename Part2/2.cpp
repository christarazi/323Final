/* So far this works for the input file "finalp2.txt" and detecting the keywords: "program", "begin" and "end.". */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
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

	return 0;
}