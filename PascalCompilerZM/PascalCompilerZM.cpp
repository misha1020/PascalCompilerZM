#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

char NextCh();

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream fErrMsgs("Err.msg");
	string *errorsArray = new string[500];
	int errCount = 0;
	while (getline(fErrMsgs, errorsArray[errCount]))
	{
		errCount++;
	}
	fErrMsgs.close();
	//ifstream fPascalCode("PascalCode.txt");
	//ifstream fErrors("Errors.txt");

	for (int i = 0; i < errCount; i++)
		cout << "Считанная строка: " << errorsArray[i] << endl;

	system("pause");
	return 0;
}

char NextCh()
{
	return 'S';
}