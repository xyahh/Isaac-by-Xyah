#include "stdafx.h"
#include "DevConsole.h"
#include <fcntl.h>
#include <io.h>

/*
Code Source: https://stackoverflow.com/a/43870739
*/

#pragma warning(push)
#pragma warning(disable:4311)
long HTL(HANDLE ht) { return reinterpret_cast<long>(ht); }
#pragma warning(pop)

void DevConsole::Create()
{
	AllocConsole();
	std::wstring Title = L"DevConsole";
	SetConsoleTitle(Title.c_str());
	typedef struct 
	{ 
		char* _ptr; 
		int _cnt; 
		char* _base; 
		int _flag; 
		int _file; 
		int _charbuf; 
		int _bufsiz; 
		char* _tmpfname; 
	} FILE_COMPLETE;

	*(FILE_COMPLETE*)stdout = *(FILE_COMPLETE*)_fdopen(_open_osfhandle(HTL(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT), "w");
	*(FILE_COMPLETE*)stderr = *(FILE_COMPLETE*)_fdopen(_open_osfhandle(HTL(GetStdHandle(STD_ERROR_HANDLE)), _O_TEXT),  "w");
	*(FILE_COMPLETE*)stdin =  *(FILE_COMPLETE*)_fdopen(_open_osfhandle(HTL(GetStdHandle(STD_INPUT_HANDLE)), _O_TEXT),  "r");

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);

	EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
}