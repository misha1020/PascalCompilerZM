#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream inFile("input.txt");
	char symbol;
	string out = "";
	inFile >> symbol;
	while (!inFile.eof())
	{
		out += symbol;
		inFile >> symbol;
	}
	inFile.close();

	cout << "Считанная строка: " << out << endl; 

	system("pause");
	return 0;
}