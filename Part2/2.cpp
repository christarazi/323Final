/* So far this works for the input file "finalp2.txt" and detecting the keywords: "program", "begin" and "end.". */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <list>
#include <algorithm>

using namespace std;

void detectKeywords(list<string> l)
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

int main()
{
	list<string> l;
	fstream fIn("finalp2.txt", ios::in);
	string line;

	while(getline(fIn, line))
	{
		l.push_back(line);
	}

	fIn.close();

	detectKeywords(l);

	return 0;
}