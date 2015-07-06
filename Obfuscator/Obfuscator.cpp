// Obfuscator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

map<string, string> constants;

string ToLowerCase(const string &codeString)
{
	string result;
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		result.push_back(tolower(codeString[i]));
	}
	return result;
}

size_t ReturnNonSpacePlace(const string &processedString)
{
	for (size_t nonSpacePlace = 0; nonSpacePlace < processedString.size(); ++nonSpacePlace)
	{
		if (processedString[nonSpacePlace] != ' ')
		{
			return nonSpacePlace;
		}
	}

	return 0;
}

void CopyToResultString(string &resultString, const string &originalString, size_t elementPosition, int &spacesCounter)
{
	if (originalString[elementPosition] == ' ')
	{
		++spacesCounter;
	}
	else
	{
		if (spacesCounter > 1)
		{
			resultString += " ";
			resultString += originalString[elementPosition];
			spacesCounter = 0;
		}
		else
		{
			resultString += originalString[elementPosition];
			spacesCounter = 0;
		}
	}
}

string RemoveExtraSpaces(const string &processedString)
{
	string resultString;
	if (processedString.size())
	{
		int spacesCounter = 0;
		size_t nonSpacePlace = ReturnNonSpacePlace(processedString);
		for (size_t stringElementPosition = nonSpacePlace; stringElementPosition < processedString.size(); ++stringElementPosition)
		{
			CopyToResultString(resultString, processedString, stringElementPosition, spacesCounter);
		}
	}

	return resultString;
}

void FillConstants(const vector<string> &code, size_t &line)
{
	string codeString = code[line];
	string compare = ToLowerCase(codeString);
	while (compare != "var" || compare != "type")
	{
		istringstream is(codeString);
		string s;
		vector<string> arrayStrings;
		while (getline(is, s, '='))
		{
			arrayStrings.push_back(s);
		}
		
		for (size_t i = 0; i < arrayStrings.size(); ++i)
		{
			arrayStrings[i] = RemoveExtraSpaces(arrayStrings[i]);
		}

		arrayStrings[1].erase(arrayStrings[1].end() - 1);

		constants[arrayStrings[0]] = arrayStrings[1];
		++line;
		codeString = code[line];
		compare = ToLowerCase(codeString);
	}
}

bool IsEndString(const string &codeString)
{
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		if (codeString[i] == ';')
		{
			return true;
		}
	}
	return false;
}

void ReadCode(const string inputFileName, vector<string> &code)
{
	ifstream inputFile;
	inputFile.open(inputFileName, ifstream::in);

	if (!inputFile.good())
	{
		cout << "Error: cannot open input file!" << endl;
		exit(1);
	}
	bool endStringFlag;
	while (inputFile.good())
	{
		string codeString;
		getline(inputFile, codeString);

		endStringFlag = IsEndString(codeString);

		istringstream is(codeString);
		string s;
		vector<string> arrayStrings;
		while (getline(is, s, ';'))
		{
			arrayStrings.push_back(s);
		}

		for (size_t i = 0; i < arrayStrings.size(); ++i)
		{
			if (endStringFlag)
			{
				arrayStrings[i].push_back(';');
			}
			
			code.push_back(arrayStrings[i]);
		}
	}
}

void FillVars(const vector<string> &code, size_t &line)
{

}

void ParseCode(const vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		string compare = ToLowerCase(code[i]);
		if (compare == "const")
		{
			++i;
			FillConstants(code, i);
		}
		compare = ToLowerCase(code[i]);
		if (compare == "var")
		{
			++i;
			FillVars(code, i);
		}
	}
}

int main(int argc, char* argv[])
{
	string inputFileName = argv[1];
	vector<string> code;
	map<string, string> constants;
	ReadCode(inputFileName, code);
	ParseCode(code);
	return 0;
}

