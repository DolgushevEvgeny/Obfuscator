// Obfuscator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

using namespace std;

map<string, string> constants;
map<string, string> variables;
map<string, string> procsAndFuncs;
bool comment_flag = false;

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
			if (spacesCounter == 1)
			{
				resultString += " ";
			}
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
	while ((compare != "var") && (compare != "type") && (compare.size()))
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
		compare = RemoveExtraSpaces(compare);
	}
	--line;
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
		codeString = RemoveExtraSpaces(codeString);
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

bool CheckForNewVariable(const string &newVariable)
{
	map<string, string>::iterator element;
	for (element = variables.begin(); element != variables.end(); ++element)
	{
		if (element->second == newVariable)
		{
			return true;
		}
	}
	return false;
}

bool CheckForNewProcsAndFuncs(const string &newElement)
{
	map<string, string>::iterator element;
	for (element = procsAndFuncs.begin(); element != procsAndFuncs.end(); ++element)
	{
		if (element->second == newElement)
		{
			return true;
		}
	}
	return false;
}

string CreateNewVariable(void)
{
	string result;
	bool compare = true;
	srand(time(0));

	while (compare)
	{
		result.clear();
		size_t size = rand() % 10 + 1;
		int letter = rand() % 26 + 97;
		int toLower = rand() % 2;
		if (toLower)
		{
			result.push_back(toupper(char(letter)));
		}
		else
		{
			result.push_back(char(letter));
		}

		for (size_t i = 0; i < size - 1; ++i)
		{
			int letterType = rand() % 2;

			if (letterType)
			{
				letter = rand() % 26 + 97;
				toLower = rand() % 2;
				if (toLower)
				{
					result.push_back(toupper(char(letter)));
				}
				else
				{
					result.push_back(char(letter));
				}
			}
			else
			{
				int digit = rand() % 10 + 48;
				result.push_back(digit);
			}
		}

		compare = CheckForNewVariable(result);
	}

	return result;
}

void AddVariables(const string &codeString)
{
	string originalVariable, newVariable;
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		if (codeString[i] == ':')
		{
			break;
		}
		else
		{
			while ((codeString[i] != ',') && (codeString[i] != ':'))
			{
				originalVariable.push_back(codeString[i]);
				++i;
			}
			if ((codeString[i] != ','))
			{
				--i;
			}
			originalVariable = RemoveExtraSpaces(originalVariable);
			newVariable = CreateNewVariable();

			variables[originalVariable] = newVariable;
		}

		originalVariable.clear();
		newVariable.clear();
	}
}

void FillVars(const vector<string> &code, size_t &line)
{
	string codeString = code[line];
	string compare = ToLowerCase(codeString);
	compare = RemoveExtraSpaces(compare);
	while (compare != "begin")
	{
		string temp = RemoveExtraSpaces(compare);
		string str = temp.substr(0,9);
		str = ToLowerCase(str);
		if (str == "procedure")
		{
			--line;
			return;
		}
		str.erase(str.end() - 1);
		if (str == "function")
		{
			--line;
			return;
		}
		AddVariables(codeString);

		++line;
		codeString = code[line];
		compare = ToLowerCase(codeString);
		compare = RemoveExtraSpaces(compare);
	}
}

string CreateNewProceduresAndFunctionsName(void)
{
	string result;
	bool compare = true;
	srand(time(0));

	while (compare)
	{
		result.clear();
		size_t size = rand() % 10 + 1;
		int letter = rand() % 26 + 97;
		int toLower = rand() % 2;
		if (toLower)
		{
			result.push_back(toupper(char(letter)));
		}
		else
		{
			result.push_back(char(letter));
		}

		for (size_t i = 0; i < size - 1; ++i)
		{
			if (i % 2 == 0)
			{
				result.push_back('_');
				++i;
			}

			if (i < size - 1)
			{
				int letterType = rand() % 2;

				if (letterType)
				{
					letter = rand() % 26 + 97;
					toLower = rand() % 2;
					if (toLower)
					{
						result.push_back(toupper(char(letter)));
					}
					else
					{
						result.push_back(char(letter));
					}
				}
				else
				{
					int digit = rand() % 10 + 48;
					result.push_back(digit);
				}
			}
		}

		compare = CheckForNewProcsAndFuncs(result);
	}

	return result;
}

void FillProcsAndFuncs(const string &codeString, const size_t position)
{
	string originalName, newName;
	for (size_t i = position; i < codeString.size(); ++i)
	{
		if ((codeString[i] != '(') && (codeString[i] != ';'))
		{
			originalName.push_back(codeString[i]);
		}
		else
		{
			break;
		}
	}

	newName = CreateNewProceduresAndFunctionsName();
	procsAndFuncs[originalName] = newName;
}

void CheckOnBrackets(const string &codeString)
{
	size_t left = 0, right = 0;
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		if (codeString[i] == '(')
		{
			left = i + 1;
		}
		if (codeString[i] == ')')
		{
			right = i;
		}
	}

	if (left && right)
	{
		string temp = codeString.substr(left, right - left - 1);
		istringstream is(temp);
		string s;
		vector<string> arrayStrings;
		while (getline(is, s, ';'))
		{
			arrayStrings.push_back(s);
		}

		for (size_t j = 0; j < arrayStrings.size(); ++j)
		{
			AddVariables(arrayStrings[j]);
		}
	}
}

string DeleteOneStringComment(const string &codeString)
{
	bool wasSlesh = false;
	bool inString = false;
	string result;
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		if (codeString[i] == '\'' && inString)
		{
			inString = false;
		}
		if (codeString[i] == '\'' && !inString)
		{
			inString = true;
		}
		if (!inString)
		{
			if (codeString[i] == '/')
			{
				if (wasSlesh)
				{
					result.push_back(' ');
					return result;
				}
				else
				{
					wasSlesh = true;
				}
			}
			else
			{
				if (wasSlesh)
				{
					wasSlesh = false;
				}
				result.push_back(codeString[i]);
			}
		}
		if (inString)
		{
			result.push_back(codeString[i]);
		}
	}
	result.push_back(' ');
	return result;
}

string DeleteMultiStringComment(const string &codeString)
{
	string result = "";
	bool wasBracket = false;
	for (size_t i = 0; i < codeString.size(); ++i)
	{
		if (!comment_flag)
		{
			if (!wasBracket)
			{
				if (codeString[i] == '{')
				{
					wasBracket = true;
					comment_flag = true;
				}
				else
				{
					result.push_back(codeString[i]);
				}
			}
			else
			{
				if (codeString[i] == '}')
				{
					wasBracket = false;
				}
			}
		}
		else
		{
			if (codeString[i] == '}')
			{
				wasBracket = false;
				comment_flag = false;
			}
		}
	}

	return result;
}

void ExpandCycleFOR(vector<string> &code, const string &cycleType, const string &variable, const string &left, const string &right, bool isInteger, size_t &line)
{
	++line;
	string codeString = ToLowerCase(code[line]);
	codeString = RemoveExtraSpaces(codeString);

	size_t start, end;
	int first, second;
	if (codeString == "begin")
	{
		start = line;
		int cycles = 1;
		while (cycles)
		{
			++line;
			string codeString = ToLowerCase(code[line]);
			codeString = RemoveExtraSpaces(codeString);
			if (codeString == "begin")
			{
				++cycles;
			}
			codeString.erase(codeString.end() - 1);
			if (codeString == "end")
			{
				--cycles;
			}
		}
		end = line;

		vector<string> cyclesArray;
		string temp;
		if (isInteger)
		{
			temp += variable;
			temp += " := ";
			temp += left;
			temp += ";";
		}
		else
		{
			temp += variable;
			temp += " := ";
			temp += left;
			temp += ";";
		}
		
		cyclesArray.push_back(temp);
		for (size_t i = start; i < end + 1; ++i)
		{
			cyclesArray.push_back(code[i]);
		}
		
		string temp1;
		if (cycleType == "to")
		{
			temp1 += ("inc(variable);");
		}
		if (cycleType == "downto")
		{
			temp1+= ("dec(variable);");
		}
		cyclesArray.push_back(temp1);
	}
	else
	{

	}
}

void ParseCode(const vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		string compare = ToLowerCase(code[i]);
		compare = RemoveExtraSpaces(compare);
		if (compare.size() > 1)
		{
			//string compare = ToLowerCase(code[i]);
			//compare = RemoveExtraSpaces(compare);
			if (compare == "const")
			{
				++i;
				FillConstants(code, i);
			}
			else
			{
				//compare = ToLowerCase(code[i]);
				if (compare == "var")
				{
					++i;
					FillVars(code, i);
				}
				else
				{
					string temp = RemoveExtraSpaces(compare);
					string str = temp.substr(0, 9);
					str = ToLowerCase(str);
					if (str == "procedure")
					{
						FillProcsAndFuncs(code[i], str.size() + 1);
						CheckOnBrackets(code[i]);
					}
					str.erase(str.end() - 1);
					if (str == "function")
					{
						FillProcsAndFuncs(code[i], str.size() + 1);
						CheckOnBrackets(code[i]);
					}	
				}
			}
		}
	}
}

bool isVariableInteger(const string &codeString)
{
	if (isdigit(codeString[0]))
	{
		return true;
	}

	return false;
}

void ChangeCycleForWHILE()
{

}

void ChooseTypeOfCycle(vector<string> &code, size_t &line)
{
	string codeString = ToLowerCase(code[line]);
	codeString = RemoveExtraSpaces(codeString);
	int first_1 = 0, second_1 = 0, difference = 0;
	string first, second, cycleType, variable;

	///////////
	istringstream is(codeString);
	string s;
	vector<string> arrayStrings;
	while (getline(is, s, ' '))
	{
		arrayStrings.push_back(s);
	}
	///////////


	variable = arrayStrings[1];
	cycleType = arrayStrings[4];

	///////////
	if (isVariableInteger(arrayStrings[3]))
	{
		first_1 = atoi(arrayStrings[3].c_str());
	}
	else
	{
		first = arrayStrings[3];
	}
	if (isVariableInteger(arrayStrings[5]))
	{
		second_1 = atoi(arrayStrings[5].c_str());
	}
	else
	{
		second = arrayStrings[5];
	}
	///////////

	if (first_1 && second_1)
	{
		difference = abs(second_1 - first_1);
		if (difference <= 5 && difference > 0)
		{
			ExpandCycleFOR(code, cycleType, variable, arrayStrings[3], arrayStrings[5], true, line);
		}
		else
		{
			ChangeCycleForWHILE();/////////////
		}
	}

}

void WatchCyclesFOR(vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		string codeString = ToLowerCase(code[i]);
		codeString = RemoveExtraSpaces(codeString);
		if (codeString.size() > 2)
		{
			string temp(codeString);
			temp.erase(temp.begin() + 3, temp.end());
			if (temp == "for")
			{
				ChooseTypeOfCycle(code, i);
			}
		}
	}
}

void DeleteOneStringComment(vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		code[i] = DeleteOneStringComment(code[i]);
	}
}

void DeleteMultiStringComment(vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		code[i] = DeleteMultiStringComment(code[i]);
	}
}

void SafeCode(const vector<string> &code, const string &inputFileName)
{
	ofstream outputFile;
	outputFile.open(inputFileName, ofstream::out);

	for (size_t i = 0; i < code.size(); ++i)
	{
		if (code[i].size() > 1)
		{
			outputFile << code[i] << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	string inputFileName = argv[1];
	vector<string> code;
	map<string, string> constants;
	
	ReadCode(inputFileName, code);
	DeleteOneStringComment(code);
	DeleteMultiStringComment(code);
	ParseCode(code);
	WatchCyclesFOR(code);
	SafeCode(code, inputFileName);
	return 0;
}

