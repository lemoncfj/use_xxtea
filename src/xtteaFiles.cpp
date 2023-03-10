#include <iostream>
#include <cstring>        // for strcpy(), strcat()
#include<direct.h>
#include <io.h>

using namespace std;

extern int encryptFiles();
extern int decodetFiles();
int main()
{
	char option[512] = { 0 };
	cout << "Enter a options:" <<endl;
	cout << "1 --encrypt files" << endl;
	cout << "2 --decode files" << endl;
	cin.getline(option, 512);
	if (strcmp(option, "1") == 0) {
		encryptFiles();
	}
	if (strcmp(option, "2") == 0) {
		decodetFiles();
	}	
	return 0;
}
