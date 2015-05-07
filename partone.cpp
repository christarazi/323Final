#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <cctype>
#include <deque>
#include "partone.h"

using namespace std;

// Function to initiate and terminate the parsing process.
void initiateParsing()
{
	deque<string> q;
	deque<string> finalQ;
	fstream fOut;
	fOut.open("finalp2.txt", ios::out);

	parseFile(q);

	// Iterate through queue to write to output file.
	for (auto&& i : q) if (i != "\n") fOut << i;
	fOut.close();
}

// Function to parse input file. Takes a queue by reference.
void parseFile(deque<string> & q)
{
	fstream fIn, fOut;
	string line;

	// Our input file is "finalp1.txt".
	fIn.open("finalp1.txt", ios::in);

	while (getline(fIn, line))
	{
		// Declare regex patterns.
		regex whitespace("[\\s]+");			// Looks for one or more whitespace chars.
		regex leadingSpace("^(\\s+)");			// Looks for one or more leading spaces.
		regex semiColonNewLine("([ \t]?;[ \t]?)");	// Looks for ';' surrounded by zero or more spaces.
		regex comments("(((\\(\\*.*\\*\\))|(\\(\\*.*))|(.*\\*\\)))");	// Looks for comments.
		regex reservedWords("((var)|(begin)|(BEGIN)|(VAR))");	// Looks for reserved words.
		regex endWord("(end\\.)|(END\\.)");	// Looks for "end.".
		regex everyLineMustBeNewLine("^(?!var|VAR|BEGIN|begin).*?$");	// Matches every line except ones that start with reserved words.

        // Replaces 'line' with single whitespace.
		line = regex_replace(line, whitespace, " ");			
        // Replaces 'line' in order to not have leading spaces.
		line = regex_replace(line, leadingSpace, "");
        // Replaces 'line' with lines that have semicolons then newline.
		line = regex_replace(line, semiColonNewLine, " ;\n");
		// Replaces 'line' with "end." plus newline.
		line = regex_replace(line, endWord, "end.\n");
		// Replaces 'line' with nothing because of comments.
		line = regex_replace(line, comments, "");
		// Replaces 'line' with the itself plus newline.
		line = regex_replace(line, reservedWords, line+"\n");
		// Replaces 'line' with the itself plus newline. Makes sure every line (even w/o ';') has its own line.
		line = regex_replace(line, everyLineMustBeNewLine, line+"\n");

		// Check if the lines starts with a comment, if so, proceed; does not get pushed to queue.
		if (line[0] == '(' && line[1] == '*')
		{
			continue;
		}

		// Rebuild 'line' using temporary string variable.
		string temp = "";

		// Process each line by character manually.
		for (int i = 0; i < line.length(); ++i)
		{
			// If the current character is...
			if (line[i] == ',' || line[i] == '+' || line[i] == '-' ||
				line[i] == '=' || line[i] == '*' || line[i] == '/' ||
				line[i] == '(' || line[i] == ')')
			{
				// and if both previous and next character are not spaces...
				if (line[i-1] != ' ' && line[i+1] != ' ')
				{
					// pad both sides.
					temp += ' ';
					temp += line[i];
					temp += ' ';
				}

				// else if next character is not a space...
				else if (line[i+1] != ' ')
				{
					// pad after the current character.
					temp += line[i];
					temp += ' ';
				}

				// else if previous character is not a space...
				else if (line[i-1] != ' ')
				{
					// add space to 'temp'; this pads the file with a space before char listed above.
					temp += ' ';

					// Concatenate temp with current character.
					temp += line[i];
				}

				else
				{
					// Concatenate temp with current character.
					temp += line[i];
				}

				// If next character is a space and current character is a semicolon...
				if (line[i+1] == ' ' && line[i] == ';')
				{
					// pad 'temp' with a space.
					temp += ' ';
				}
			}

			// Else if current char is a '/' as well as next char... (this means it's a comment)
			else if (line[i] == '(' && line[i+1] == '*')
			{
				// Break out of for loop; we do not want comments.
				break;
			}

			// Finally, concatenate current character.
			else temp += line[i];
		}

		// Push the line we just processed ('temp') to queue.
		q.push_back(temp);
	}

	// Close the input file.
	fIn.close();
}