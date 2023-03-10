#include <iostream>
#include <cstring>        // for strcpy(), strcat()
#include<direct.h>
#include <io.h>

using namespace std;

extern int check_mkDirs(const char* dir, char* outDir);
extern int mkDirs(const char* dir, char* outDir);
extern int isFileType(_finddata_t findData, const char* fileType);
int decodetFiles(const char * dir, const char * fileType, const char * outDir);
int decodetFiles(){
	char dir[512] = { 0 };
	char fileType[64] = { 0 };
	char outDir[512] = { 0 };
	char outStr[12] = { 0 };

	cout << "Enter a decodet directory: ";
	cin.getline(dir, 512);
	//_getcwd(dir, sizeof(dir) - 1);
	cout << dir << endl;
	cout << "Enter a file type example(* --all, png jpg ..)";
	cin.getline(fileType, 64);
	cout << "Enter a decodet ouput directory: ";
	cin.getline(outDir, 512);
	int r = check_mkDirs(dir, outDir);
	if (r != 0) {
		cout << "check_mkDirs failed" << endl;
		return 0;
	}
	r = mkDirs(dir, outDir);
	if (r != 0) {
		cout << "mkDirs failed" << endl;
		return 0;
	}
	r = decodetFiles(dir, fileType, outDir);
	if (r != 0) {
		cout << dir << "not exist" << endl;
		return 0;
	}
	cout << "Enter any key out ";
	cin.getline(outStr, 12);
	return 0;
}

int decodetFiles(const char * dir, const char* fileType, const char* outDir)
{
	char outDirNew[1204];
	char dirNew[512];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1) {       // 检查是否成功
		return 1;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0) {
				continue;
			}
			cout << findData.name << "\t<dir>\n";
			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);
			strcpy(outDirNew, outDir);
			strcat(outDirNew, "\\");
			strcat(outDirNew, findData.name);
			decodetFiles(dirNew, fileType, outDirNew);
		}
		else {
			int r = isFileType(findData, fileType);
			if (r == 0) {
				char cmd[2048] = { 0 };
				char inFileName[512] = { 0 };
				char outFileName[512] = { 0 };
				sprintf(inFileName, "%s\\%s", dir, findData.name);
				sprintf(outFileName, "%s\\%s", outDir, findData.name);
				sprintf(cmd, "xxtea.exe -d -i %s -s 123zttDev.com -o %s ", inFileName, outFileName);
				system(cmd);
				cout << endl;
				cout << findData.name << "\t" << findData.size << " bytes.\n";
			}
		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // 关闭搜索句柄
	return 0;
}