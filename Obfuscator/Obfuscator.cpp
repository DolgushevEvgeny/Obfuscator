// Obfuscator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

string ToLowerCase(const string &codeString)
{
	string result;
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		result.push_back(tolower(codeString[i]));
	}
	return result;
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

		string compare = ToLowerCase(codeString);
		if (compare == "const")
		{

		}
		code.push_back(codeString);
	}
}

int main(int argc, char* argv[])
{
	string inputFileName = argv[1];
	vector<string> code;
	ReadCode(inputFileName, code);
	return 0;
}

