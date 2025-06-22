#include <iostream>
#include <string>
#include <fstream>
using namespace std;


void intertext_to_text()
{
	/*	"\intertext{}" is replaced by "\text{}" and "intertext{asd}" is replaced by "\text{}\\\text{asd}\\text{}\\".	*/
	
	//	string str = "\\begin{align*}\n\tm_0 &= 1 & m_0' &= 1 \\\\\n\tm_1 &= 0 & m_1' &= 3 \\\\\n\t\\intertext{}\n\tm_2 &= 1.5 & m_2' &= 10.5 \\\\\n\t\\intertext{asd}\n\tm_3 &= 0 & m_3' &= 40.5 \\\\\n\\end{align*}\n\n\\begin{align*}\n\tm_0 &= 1 & m_0' &= 1 \\\\\n\tm_1 &= 0 & m_1' &= 3 \\\\\n\t\\intertext{}\n\tm_2 &= 1.5 & m_2' &= 10.5 \\\\\n\t\\\intertext{asd}\n\tm_3 &= 0 & m_3' &= 40.5 \\\\\n\\end{align*}";
	string str;
	str.reserve(20000);
	
//	cout << "Enter the string delimited by '@'.\n\n";	
//	getline(std::cin, str,'@');	//https://stackoverflow.com/questions/63835061/how-to-take-multiple-line-string-input-in-c
//	cout << str << "\n";

	string line;
	line.reserve(1000);
	ifstream inputFile("input_intertext_to_text.txt");
	if (inputFile.is_open())
	{
		while (getline (inputFile, line))
		{
			str += '\n';
			str += line;
			line.clear();
		}
	}
	else
	{
		cout << "Error opening read file.\n";
	}

	inputFile.close();

	int strLength = str.length();
	cout << "strLength = " << strLength << "\n";
	
	string intertext = "\\intertext{";
	
	int noOfmatches = 0;	//	"intertext{" matches
	int matchIndex = str.find(intertext);
	while(string::npos != matchIndex)
	{
		noOfmatches++;

		if ((matchIndex + 1) < str.length())
		{
			matchIndex = str.find(intertext, matchIndex + 1);
		}
	}

	matchIndex = 0;
	for(int i = 0; i < noOfmatches; i++)
	{
		matchIndex = str.find(intertext, matchIndex + 1);
		if ('}' == str[matchIndex + 11])	//Empty \intertext{}, insert an empty \text{}
		{
			str.erase(matchIndex + 1, 5);	//Erase "inter" from \intertext{}
			str.insert(matchIndex + 7, "\\\\");
		}
		else	//Non empty \intertext{}
		{
			str.insert(matchIndex, "\\text{}\\\\");	//Insert "\text{}\\"
			str.erase(matchIndex + 10, 5);	//Erase "inter" from \intertext{}
			matchIndex += 10;
			matchIndex = str.find('\n', matchIndex);	//Find newline.
			while (str[matchIndex --] != '}');		//Backtrack to find the last curly bracket.
			str.insert(matchIndex + 2, "\\\\\\text{}\\\\");	//Insert another "\text{}\\"
		}

		matchIndex += 5;
	}

	ofstream outputFile("output_intertext_to_text.txt");
	if (outputFile.is_open())
	{
		outputFile << str;
		outputFile.close();
	}
	else
	{
		cout << "Error opening output file.\n";
	}
	
	cout << "\n\n" << str << "\n";
	str.clear();
}

void handle()
{
	bool toContinue = true;
	while(toContinue)
	{
		intertext_to_text();
		char c;
		cout << "\nWould you like to add another entry? 'y'/any other:\t";
		cin >> c;
		if ('y' != c)
		{
			toContinue = false;
		}
		else
		{
			cout << "\n";
			cin.clear();	//https://stackoverflow.com/questions/14943789/cin-get-issue-with-empty-line
			cin.sync();
		}
	}	
}

int main(void)
{
	handle();

	return 0;
}