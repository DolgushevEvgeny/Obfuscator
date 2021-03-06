// Obfuscator.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

using namespace std;

map<string, string> constants;
map<string, string> variables;
map<string, string> procsAndFuncs;
set<string> keyWords;
set<char> setOfSpecialChars = { '+', '-', '/', '*', '=', '<', '>', '[', ']', '.', ',', '(', ')', ':', ';', '^', '@', '$', '#' };
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

vector<string> SplitString(const string &processedString, const char divider)
{
	istringstream is(processedString);
	string s;
	vector<string> arrayStrings;
	while (getline(is, s, divider))
	{
		arrayStrings.push_back(s);
	}
	return arrayStrings;
}

void ChooseStringAndProcessingIt(const vector<string> &code, size_t &line, string &codeString, string &compare)
{
	++line;
	codeString = code[line];
	compare = ToLowerCase(codeString);
	compare = RemoveExtraSpaces(compare);
}

void FillConstants(vector<string> &code, size_t &line)
{
	size_t start, end;
	start = line - 1;
	string codeString = code[line];
	string compare = ToLowerCase(codeString);
	while ((compare != "var") && (compare != "type") && (compare.size()))
	{
		vector<string> arrayStrings = SplitString(codeString, '=');

		for (size_t i = 0; i < arrayStrings.size(); ++i)
		{
			arrayStrings[i] = RemoveExtraSpaces(arrayStrings[i]);
		}

		arrayStrings[1].erase(arrayStrings[1].end() - 1);
		constants[arrayStrings[0]] = arrayStrings[1];

		ChooseStringAndProcessingIt(code, line, codeString, compare);
	}
	end = line;
	for (size_t i = start; i < end; ++i)
	{
		code[i] = "";
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

		vector<string> arrayStrings = SplitString(codeString, ';');

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

void ChooseSizeOfLetter(string &result, const int &letter)
{
	int toLower = rand() % 2;
	if (toLower)
	{
		result.push_back(toupper(char(letter)));
	}
	else
	{
		result.push_back(char(letter));
	}
}

void ChooseLetterInNewName(string &result)
{
	int letter = rand() % 26 + 97;
	ChooseSizeOfLetter(result, letter);
}

void ChooseNewDigitInName(string &result)
{
	int digit = rand() % 10 + 48;
	result.push_back(digit);
}

void ChooseTypeOfSymbolInNewName(string &result)
{
	int letterType = rand() % 2;

	if (letterType)
	{
		ChooseLetterInNewName(result);
	}
	else
	{
		ChooseNewDigitInName(result);
	}
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
		ChooseLetterInNewName(result);

		for (size_t i = 0; i < size - 1; ++i)
		{
			ChooseTypeOfSymbolInNewName(result);
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
		ChooseStringAndProcessingIt(code, line, codeString, compare);
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
		ChooseLetterInNewName(result);

		for (size_t i = 0; i < size - 1; ++i)
		{
			if (i % 2 == 0)
			{
				result.push_back('_');
				++i;
			}

			if (i < size - 1)
			{
				ChooseTypeOfSymbolInNewName(result);
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
		vector<string> arrayStrings = SplitString(temp, ';');

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

string CreateString(const string &variable, const string &left)
{
	string codeString;
	codeString += variable;
	codeString += " := ";
	codeString += left;
	codeString += ";";
	return codeString;
}

void ExpandCycleFOR(vector<string> &code, const string &cycleType, const string &variable, const string &left, const string &right, bool isInteger, size_t &line)
{
	size_t lineCount = line;
	++lineCount;
	string codeString = ToLowerCase(code[lineCount]);
	codeString = RemoveExtraSpaces(codeString);

	size_t start, end;
	int first, cyclesCount;
	if (codeString == "begin")
	{
		start = lineCount;
		int cycles = 1;
		while (cycles)
		{
			++lineCount;
			string codeString = ToLowerCase(code[lineCount]);
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
		end = lineCount;

		vector<string> cyclesArray;
		string temp;

		temp = CreateString(variable, left);

		cyclesArray.push_back(temp);
		for (size_t i = start; i < end + 1; ++i)
		{
			cyclesArray.push_back(code[i]);
		}
		
		cyclesCount = atoi(right.c_str());
		first = atoi(left.c_str());
		string temp1;
		if (cycleType == "to")
		{
			temp1 += ("inc(");
			temp1 += variable;
			temp1 += (");");
			cyclesCount = abs(cyclesCount - first + 1);
		}
		if (cycleType == "downto")
		{
			temp1 += ("dec(");
			temp1 += variable;
			temp1 += (");");
			cyclesCount = abs(first - cyclesCount + 1);
		}
		cyclesArray.push_back(temp1);

		for (size_t i = start - 1; i < end + 1; ++i)
		{
			code[i] = "";
		}

		vector<string>::iterator position;
		for (size_t i = 0; i < cyclesCount; ++i)
		{
			position = code.begin() + start - 1;
			if (i == 0)
			{
				code.insert(position, cyclesArray.begin(), cyclesArray.end());
			}
			else
			{
				code.insert(position + cyclesArray.size(), cyclesArray.begin() + 1, cyclesArray.end());
			}
		}
	}
	else
	{
		string result;
		bool isEndOfString = false;
		start = lineCount;
		while (!isEndOfString)
		{
			string temp = code[lineCount];
			for (size_t i = 0; i < temp.size(); ++i)
			{
				if (temp[i] != ';')
				{
					result.push_back(temp[i]);
				}
				else
				{
					result.push_back(';');
					isEndOfString = true;
					break;
				}
			}
			if (isEndOfString)
			{
				break;
			}
			++lineCount;
		}
		end = lineCount;

		vector<string> cyclesArray;
		string temp;

		temp = CreateString(variable, left);

		cyclesArray.push_back(temp);
		cyclesArray.push_back(result);

		cyclesCount = atoi(right.c_str());
		first = atoi(left.c_str());
		string temp1;
		if (cycleType == "to")
		{
			temp1 += ("inc(");
			temp1 += variable;
			temp1 += (");");
			cyclesCount = abs(cyclesCount - first + 1);
		}
		if (cycleType == "downto")
		{
			temp1 += ("dec(");
			temp1 += variable;
			temp1 += (");");
			cyclesCount = abs(first - cyclesCount + 1);
		}
		cyclesArray.push_back(temp1);

		for (size_t i = start - 1; i < end + 1; ++i)
		{
			code[i] = "";
		}

		vector<string>::iterator position;
		for (size_t i = 0; i < cyclesCount; ++i)
		{
			position = code.begin() + start - 1;
			if (i == 0)
			{
				code.insert(position, cyclesArray.begin(), cyclesArray.end());
			}
			else
			{
				code.insert(position + cyclesArray.size(), cyclesArray.begin() + 1, cyclesArray.end());
			}
		}
	}
}

void ParseCode(vector<string> &code)
{
	for (size_t i = 0; i < code.size(); ++i)
	{
		string compare = ToLowerCase(code[i]);
		compare = RemoveExtraSpaces(compare);
		if (compare.size() > 1)
		{
			if (compare == "const")
			{
				++i;
				FillConstants(code, i);
			}
			else
			{
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

void ChangeCycleForWHILE(vector<string> &code, const string &cycleType, const string &variable, const string &left, const string &right, bool isInteger, size_t &line)
{
	size_t lineCount = line;
	++lineCount;
	string codeString = ToLowerCase(code[lineCount]);
	codeString = RemoveExtraSpaces(codeString);

	size_t start, end;
	if (codeString == "begin")
	{
		start = lineCount;
		int cycles = 1;
		while (cycles)
		{
			++lineCount;
			string codeString = ToLowerCase(code[lineCount]);
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
		end = lineCount;

		vector<string> cyclesArray;
		string temp;

		temp = CreateString(variable, left);

		string temp1;
		if (cycleType == "to")
		{
			temp1 += ("While ");
			temp1 += variable;
			temp1 += (" < ");
			temp1 += right;
			temp1 += (" + 1");
			temp1 += (" Do");
		}
		if (cycleType == "downto")
		{
			temp1 += ("While ");
			temp1 += variable;
			temp1 += (" > ");
			temp1 += right;
			temp1 += (" - 1");
			temp1 += (" Do");
		}
		//////////

		cyclesArray.push_back(temp);
		cyclesArray.push_back(temp1);
		for (size_t i = start; i < end; ++i)
		{
			cyclesArray.push_back(code[i]);
		}
		temp1.clear();
		if (cycleType == "to")
		{
			temp1 += ("inc(");
			temp1 += variable;
			temp1 += (");");
		}
		if (cycleType == "downto")
		{
			temp1 += ("dec(");
			temp1 += variable;
			temp1 += (");");
		}
		cyclesArray.push_back(temp1);
		cyclesArray.push_back(code[end]);
		for (size_t i = start - 1; i < end + 1; ++i)
		{
			code[i] = "";
		}

		vector<string>::iterator position;
		position = code.begin() + start - 1;
		code.insert(position, cyclesArray.begin(), cyclesArray.end());
	}
	else
	{
		string result;
		bool isEndOfString = false;
		start = lineCount;
		while (!isEndOfString)
		{
			string temp = code[lineCount];
			for (size_t i = 0; i < temp.size(); ++i)
			{
				if (temp[i] != ';')
				{
					result.push_back(temp[i]);
				}
				else
				{
					result.push_back(';');
					isEndOfString = true;
					break;
				}
			}
			if (isEndOfString)
			{
				break;
			}
			++lineCount;
		}
		end = lineCount;

		vector<string> cyclesArray;
		string temp;

		temp = CreateString(variable, left);

		string temp1;
		if (cycleType == "to")
		{
			temp1 += ("While ");
			temp1 += variable;
			temp1 += (" < ");
			temp1 += right;
			temp1 += (" + 1");
			temp1 += (" Do");
		}
		if (cycleType == "downto")
		{
			temp1 += ("While ");
			temp1 += variable;
			temp1 += (" > ");
			temp1 += right;
			temp1 += (" - 1");
			temp1 += (" Do");
		}
		//////////

		cyclesArray.push_back(temp);
		cyclesArray.push_back(temp1);
		cyclesArray.push_back("Begin");
		cyclesArray.push_back(result);

		temp1.clear();
		if (cycleType == "to")
		{
			temp1 += ("inc(");
			temp1 += variable;
			temp1 += (");");
		}
		if (cycleType == "downto")
		{
			temp1 += ("dec(");
			temp1 += variable;
			temp1 += (");");
		}
		cyclesArray.push_back(temp1);
		cyclesArray.push_back("End;");
		for (size_t i = start - 1; i < end + 1; ++i)
		{
			code[i] = "";
		}
		vector<string>::iterator position;
		position = code.begin() + start - 1;
		code.insert(position, cyclesArray.begin(), cyclesArray.end());
	}
}

void CalculateOneBorderOfRange(const string &codeString, int &first_1, string &first)
{
	if (isVariableInteger(codeString))
	{
		first_1 = atoi(codeString.c_str());
	}
	else
	{
		first = codeString;
	}
}

void ChooseTypeOfCycle(vector<string> &code, size_t &line)
{
	string codeString = ToLowerCase(code[line]);
	codeString = RemoveExtraSpaces(codeString);
	int first_1 = 0, second_1 = 0, difference = 0;
	string first, second, cycleType, variable;

	vector<string> arrayStrings = SplitString(codeString, ' ');

	variable = arrayStrings[1];
	cycleType = arrayStrings[4];

	CalculateOneBorderOfRange(arrayStrings[3], first_1, first);
	CalculateOneBorderOfRange(arrayStrings[5], second_1, second);

	if (first_1 && second_1)
	{
		difference = abs(second_1 - first_1);
		if (difference <= 4 && difference > 0)
		{
			ExpandCycleFOR(code, cycleType, variable, arrayStrings[3], arrayStrings[5], true, line);
		}
		else
		{
			ChangeCycleForWHILE(code, cycleType, variable, arrayStrings[3], arrayStrings[5], true, line);
		}
	}
	else
	{
		ChangeCycleForWHILE(code, cycleType, variable, arrayStrings[3], arrayStrings[5], false, line);
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
			outputFile << code[i];
		}
	}
}

void WriteWordInRandom(string &lexem)
{
	string result;
	for (size_t i = 0; i < lexem.size(); ++i)
	{
		if ((tolower(lexem[i]) >= 'a' && tolower(lexem[i]) <= 'z'))
		{
			ChooseSizeOfLetter(result, lexem[i]);
		}
		else
		{
			result.push_back(lexem[i]);
		}
	}
	lexem = result;
}

void ChooseTypeOfLexem(string &lexem, string &result)
{
	string codeString = ToLowerCase(lexem);
	codeString = RemoveExtraSpaces(codeString);

	if (keyWords.find(codeString) != keyWords.end())
	{
		result.push_back(' ');
		WriteWordInRandom(lexem);
		result += lexem;
		result.push_back(' ');
		return;
	}
	else
	{
		if (constants.find(codeString) != constants.end())
		{
			string change = constants[codeString];
			WriteWordInRandom(change);
			result += change;
			return;
		}
		else
		{
			if (variables.find(codeString) != variables.end())
			{
				string change = variables[codeString];
				WriteWordInRandom(change);
				result += change;
				return;
			}
			else
			{
				if (procsAndFuncs.find(codeString) != procsAndFuncs.end())
				{
					string change = procsAndFuncs[codeString];
					WriteWordInRandom(change);
					result += change;
					return;
				}
				else
				{
					WriteWordInRandom(codeString);
					result += codeString;
					return;
				}
			}
		}
	}
}

void CreateNewCode(vector<string> &code)
{
	bool wasQuote = false;
	string lexem, newCodeString;
	for (size_t i = 0; i < code.size(); ++i)
	{
		string result, codeString = code[i];
		for (size_t j = 0; j < codeString.size(); ++j)
		{
			if (!wasQuote)
			{
				if (codeString[j] == '\'')
				{
					wasQuote = true;
					ChooseTypeOfLexem(lexem, result);
					newCodeString += result;
					lexem.clear();
					result.clear();
					newCodeString.push_back(codeString[j]);
				}
				else
				{
					if (codeString[j] != ' ')
					{
						if ((tolower(codeString[j]) >= 'a' && tolower(codeString[j]) <= 'z'))
						{
							lexem.push_back(codeString[j]);
						}
						else
						{
							if (codeString[j] >= '0' && codeString[j] <= '9')
							{
								lexem.push_back(codeString[j]);
							}
							else
							{
								if (setOfSpecialChars.find(codeString[j]) != setOfSpecialChars.end())
								{
									ChooseTypeOfLexem(lexem, result);
									newCodeString += result;
									lexem.clear();
									result.clear();
									newCodeString.push_back(codeString[j]);
								}
								else
								{
									lexem.push_back(codeString[j]);
								}
							}
						}
					}
					else
					{
						ChooseTypeOfLexem(lexem, result);
						newCodeString += result;
						lexem.clear();
						result.clear();
					}
				}
			}
			else
			{
				wasQuote = false;
				newCodeString.push_back(codeString[j]);
			}
		}
		ChooseTypeOfLexem(lexem, result);
		newCodeString += result;
		code[i] = newCodeString;
		newCodeString.clear();
		lexem.clear();
		result.clear();
	}
}

int main(int argc, char* argv[])
{
	string inputFileName = argv[1];
	vector<string> code;
	map<string, string> constants;
		
	vector<string> initKeyWords =
	{ "and", "exports", "mod", "shr", "array", "file", "nil", "string",
	"asm", "for", "not", "then", "begin", "function", "object", "to",
	"case", "goto", "of", "type", "const", "if", "or", "unit", "constructor",
	"implementation", "packed", "until", "destructor", "in", "procedure", "uses",
	"div", "inherited", "program", "var", "do", "inline", "record", "while",
	"downto", "interface", "repeat", "with", "else", "label", "set", "xor",
	"end", "library", "shl", "real", "word", "double", "integer", "byte",
	"shortint", "smallint", "longword", "int64", "qword", "extended", "single",
	"real48", "boolean", "char", "readln", "writeln", "read", "write", "text", "crt",
	"abs", "sin", "cos", "arctan", "sqrt", "sqr", "power", "exp", "ln", "frac",
	"lnt", "random", "succ", "pred", "inc", "dec", "str", "val", "trunc", "round",
	"odd", "chr", "ord", "length", "concat", "copy", "delete", "new", "dispose",
	"insert", "pos", "assign", "rewrite", "close", "reset", "seek", "true", "false",
	"exit", "fileexists", "eof", "halt", "clrscr" };

	for (size_t i = 0; i < initKeyWords.size(); ++i)
	{
		keyWords.insert(initKeyWords[i]);
	}

	ReadCode(inputFileName, code);
	DeleteOneStringComment(code);
	DeleteMultiStringComment(code);
	ParseCode(code);
	WatchCyclesFOR(code);
	CreateNewCode(code);
	SafeCode(code, inputFileName);
	return 0;
}

