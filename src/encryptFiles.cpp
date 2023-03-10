#include <iostream>
#include <cstring>        // for strcpy(), strcat()
#include<direct.h>
#include <io.h>

using namespace std;

int check_mkDirs(const char* dir, char* outDir);
int mkDirs(const char* dir, char* outDir);
int encryptFiles(const char * dir, const char * fileType, const char * outDir);
int encryptFiles()
{
	char dir[512] = { 0 };
	char fileType[64] = { 0 };
	char outDir[512] = { 0 };
	char outStr[12] = { 0 };

	cout << "Enter a encrypt directory: ";
	cin.getline(dir, 512);
	//_getcwd(dir, sizeof(dir) - 1);
	cout << dir << endl;
	cout << "Enter a file type example(* --all, png jpg..): ";
	cin.getline(fileType, 64);
	cout << "Enter a encrypt ouput directory: ";
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
	r = encryptFiles(dir, fileType, outDir);
	if (r != 0) {
		cout << dir << "not exist" << endl;
		return 0;
	}
	cout << "Enter any key out ";
	cin.getline(outStr, 12);
	return 0;
}

int isFileType(_finddata_t findData, const char* fileType) {
	int len1 = strlen(findData.name);
	int len2 = strlen(fileType);
	if (len2 <= 0) {
		return 0;
	}
	if (len1 < (len2 + 1)) {
		return 1;
	}
	int index = (len1 - len2) - 1;
	char fC = findData.name[index];
	if (fC != '.') {
		return 2;
	}
	for (int i = 0; i < (len2 - 1); i++) {
		int index = (len1 - len2) + i;
		char fC1 = findData.name[index];
		char fC2 = fileType[i];
		if (fC1 != fC2) {
			return 3;
		}
	}
	return 0;
}

int isSubStr(const char* mMain, const char* mSub) {
	if (mMain == nullptr) {
		return 1;
	}
	if (mSub == nullptr) {
		return 2;
	}
	int len1 = strlen(mMain);
	int len2 = strlen(mSub);
	if (len1 <= len2) {
		for (int i = 0; i < len1 - 1; i++) {
			if (mMain[i] != mSub[i]) {
				return 3;
			}
		}
		return 0;
	}
	return 0;
}

int check_mkDirs(const char* dir, char* outDir) {
	if (strcmp(dir, outDir) == 0) {
		return 1;
	}
	int r = isSubStr(dir, outDir);
	if (r != 0) {
		return 2;
	}
	char dirNew[1024];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1) {
		return 3;
	}
	_findclose(handle);
	strcpy(dirNew, outDir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	handle = _findfirst(dirNew, &findData);
	if (handle == -1) {
		return 4;
	}
	_findclose(handle);
	return 0;
}

int mkDirs(const char* dir, char* outDir) {
	char outDirNew[1204];
	char dirNew[1024];
	strcpy(outDirNew, outDir);
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;
	int r = _access(outDir, 0);
	if (r != 0) {
		_mkdir(outDirNew);
	}
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
			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);
			strcpy(outDirNew, outDir);
			strcat(outDirNew, "\\");
			strcat(outDirNew, findData.name);
			mkDirs(dirNew, outDirNew);
		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // 关闭搜索句柄
	return 0;
}

int encryptFiles(const char * dir, const char* fileType, const char* outDir)
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
			encryptFiles(dirNew, fileType, outDirNew);
		}
		else {
			int r = isFileType(findData, fileType);
			if (r == 0) {
				char cmd[2048] = { 0 };
				char inFileName[512] = { 0 };
				char outFileName[512] = { 0 };
				sprintf(inFileName, "%s\\%s", dir, findData.name);
				sprintf(outFileName, "%s\\%s", outDir, findData.name);
				sprintf(cmd, "xxtea.exe -e -i %s -s 123zttDev.com -o %s", inFileName, outFileName);
				system(cmd);
				cout << endl;
				cout << findData.name << "\t" << findData.size << " bytes.\n";
			}
		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // 关闭搜索句柄
	return 0;
}