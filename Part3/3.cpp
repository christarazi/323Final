#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>
#include <regex>
#include <cctype>

using namespace std;

void parseStatements(deque<string> lines, deque<string> & outputLines)
{
	vector<string> parsePrint;
	regex statements("^([a-z]*?[A-Z]?[0-9]*?[ ]?=[ ]?.*)");
	regex printStatement("^print[ ]?\\(([ ]?[']?[ ]?.+?[ ]?[']?)\\)");
	smatch matches;
	for (auto&& i : lines)
	{
	    if (regex_search(i, matches, statements)) 
	    	outputLines.push_back("\t" + matches[0].str());
	    if (regex_search(i, matches, printStatement))
	    {
	    	string build = "\tcout<<";
	    	i = &i[6];
	    	parsePrint.push_back(i);
	    	for (auto&& k : parsePrint)
	    	{
	    	    for (auto&& ch : k)
	    	    {
	    	        if (ch == '\'') build += "\""; singleQuoteCounter++;
	    	        if (isalnum(ch) || ispunct(ch) && ch != ',' && ch != '(' && ch != ')' && ch != '\'')
	    	        	build += ch;
	    	        if (ch == ',') build += "<<";
	    	        if (ch == ' ') build += " ";
	    	    }
	    	}
	    	int index = build.find("  ;");
	    	build.replace(index, 10, " << endl ;");
	    	outputLines.push_back(build);
	    	parsePrint.pop_back();
	    } 
	}
}

int main()
{
	fstream fIn("finalp3.txt", ios::in);
	fstream fOut("finaloutput.cpp", ios::out);

	deque<string> lines;
	string line;
	deque<string> outputLines;

	while (getline(fIn, line)) lines.push_back(line);

	fOut << "#include <iostream>" << endl;
	fOut << "using namespace std;" << endl;
	fOut << "int main()" << endl;
	fOut << "{" << endl;

	for (auto&& i : lines)
	{
		int index = i.find(": integer");
	    if (index != -1)
	    {
	    	string temp = "\tint ";
	    	for (int j = 0; j < index; j++) temp += i[j];
	    	temp += ";";
	    	outputLines.push_back(temp);
	    	break;
	    }
	}

	parseStatements(lines, outputLines);

	outputLines.push_back("\treturn 0 ; ");
	outputLines.push_back("}");

	for (auto&& i : outputLines)
	{
		fOut << i << endl;
	}
	
	return 0;
}