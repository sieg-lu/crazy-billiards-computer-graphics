#ifndef _CUSTOM_CONSOLE_
#define _CUSTOM_CONSOLE_

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <windows.h>

class TSConsole
{
public:
	TSConsole();
	TSConsole(LPCTSTR lpszTitle, SHORT ConsoleHeight = 300, SHORT ConsoleWidth = 80);
	~TSConsole();

private:
	void Attach(SHORT ConsoleHeight, SHORT ConsoleWidth);
	static BOOL IsExistent;
};

#endif