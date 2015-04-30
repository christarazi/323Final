#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <cctype>
#include <deque>

using namespace std;

// Function to parse input file.
// Takes a queue by reference.
void parseFile(deque<string> &q)
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
		regex semiColonNewLine("(\\s*;\\s*)");	// Looks for ';' surrounded by zero or more spaces.
		regex comments("(((\\(\\*.*\\*\\))|(\\(\\*.*))|(.*\\*\\)))");	// Looks for comments.
		regex reservedWords("((var)|(begin))");	// Looks for reserved words.
		regex endWord("(end\\.)");	// Looks for "end.".

		// Process line with regex.
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
				line[i] == '=' || line[i] == '*' || line[i] == '/')
			{
				// and if both previous and next character are not spaces...
				if (line[i-1] != ' ' && line[i+1] != ' ')
				{
					// pad both sides.
					temp += ' ';
					temp += line[i];
					temp += ' ';
				}

				// else if next charcter is not a space...
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

// Function to pluck each significant word in a .cpp file.
void pluckFile()
{
	string line;
	deque<string> q;
	fstream f;

	// Opens the same file we just parsed.
	f.open("newfile.txt");

	// Iterating through every line in the file...
	while(getline(f, line))
	{
		string word;
		stringstream ss(line);

		// Iterating through every word in the line (ignoring whitespace)...
		while(ss >> word)
		{
			if (word.compare("int") == 0 || 
				word.compare("float") == 0 || 
				word.compare("double") == 0)
			{
				cout << word << " -> type\n";
			}
			else if (word.compare(",") == 0|| 
					 word.compare(";") == 0)
			{
				// No need to label these characters...
				continue;
			}
			else if (word.compare("main") == 0 ||
					 word.compare("return") == 0 || 
					 word.compare("cout<<") == 0 || 
					 word.compare("cin>>") == 0)
			{
				cout << word << " -> reserved word\n";
			}
			else if (word.compare("+") == 0 ||
					 word.compare("-") == 0 ||
					 word.compare("*") == 0 ||
					 word.compare("=") == 0)
			{
				cout << word << " -> operator\n";
			}
			else if (isdigit(word[0]))
			{
				cout << word << " -> constant\n";
			}
			else if (word.compare("{") == 0 || word.compare("}") == 0 || 
					 word.compare("(") == 0 || word.compare(")") == 0)
			{
				cout << word << " -> separator\n";
			}
			else
			{
				cout << word << " -> identifier\n";
			}
		}
	}
}
	
int main()
{
	// Our output file here is "finalp2.txt".
	fstream fOut;
	fOut.open("finalp2.txt", ios::out);

	// Declare queue of strings.
	deque<string> q;

	// Call the function to parse the file.
	parseFile(q);

	// Iterate through queue to write to output file.
	for (auto&&i : q)
	{
		fOut << i;
	}
	fOut.close();

	// Call function to pluck the file.
	//pluckFile();

	return 0;
}

