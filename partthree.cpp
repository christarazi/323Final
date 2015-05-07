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

// Function to convert all statements in the program to C++ syntax.
void parseStatements(deque<string> lines, deque<string> & outputLines)
{
	vector<string> parsePrint;
	regex statements("^([a-z]*?[A-Z]?[0-9]*?[ ]?=[ ]?.*)");
	regex printStatement("^print[ ]?\\(([ ]?[']?[ ]?.+?[ ]?[']?)\\)");
	smatch matches;
	for (auto&& line : lines)
	{
	    if (regex_search(line, matches, statements)) 
	    	outputLines.push_back("\t" + matches[0].str());
	    if (regex_search(line, matches, printStatement))
	    {
	    	string build = "\tcout<<";
	    	line = &line[6];
	    	parsePrint.push_back(line);
	    	for (auto&& k : parsePrint)
	    	{
	    	    for (auto&& ch : k)
	    	    {
	    	        if (ch == '\'') build += "\"";
	    	        if (isalnum(ch) || ispunct(ch) && ch != ',' && ch != '(' && ch != ')' && ch != '\'')
	    	        	build += ch;
	    	        if (ch == ',') build += "<<";
	    	        if (ch == ' ') build += " ";
	    	    }
	    	}
	    	int index = build.find("  ;");
	    	build.replace(index, 10, " << endl ;");	// 10 is the length of " << endl ;".
	    	outputLines.push_back(build);
	    	parsePrint.pop_back();
	    } 
	}
}

// Function that gathers all conversions made and outputs to the final text file.
void initiateConversion()
{
	fstream fIn("finalp2.txt", ios::in);
	fstream fOut("finaloutput.cpp", ios::out);

	deque<string> lines;
	deque<string> outputLines;
	string line;

	while (getline(fIn, line)) lines.push_back(line);

	fOut << "#include <iostream>" << endl;
	fOut << "using namespace std;" << endl;
	fOut << "int main()" << endl;
	fOut << "{" << endl;

	for (auto&& line : lines)
	{
		int index = line.find(": integer");
	    if (index != -1)
	    {
	    	string temp = "\tint ";
	    	for (int j = 0; j < index; j++) temp += line[j];
	    	temp += ";";
	    	outputLines.push_back(temp);
	    	break;
	    }
	}

	parseStatements(lines, outputLines);

	outputLines.push_back("\treturn 0 ; ");
	outputLines.push_back("}");

	for (auto&& line : outputLines) fOut << line << endl;
}