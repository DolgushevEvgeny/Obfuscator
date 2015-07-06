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

void FillConstants(const vector<string> &code, size_t &line)
{
	string codeString = code[line];
	string compare = ToLowerCase(codeString);
	while (compare != "var")
	{
		istringstream is(codeString);
		string s;
		vector<string> arrayStrings;
		while (getline(is, s, '='))
		{
			arrayStrings.push_back(s);
		}
		
		constants[arrayStrings[0]] = arrayStrings[1];
		++line;
		codeString = code[line];
		compare = ToLowerCase(codeString);
	}
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

	while (inputFile.good())
	{
		string codeString;
		getline(inputFile, codeString);
		code.push_back(codeString);
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
			FillConstants(code, i);
		}
		compare = ToLowerCase(code[i]);
		if (compare == "var")
		{
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
	return 0;
}

