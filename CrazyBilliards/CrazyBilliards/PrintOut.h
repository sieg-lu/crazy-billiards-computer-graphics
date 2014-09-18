#ifndef __PRINT_OUT_H__
#define __PRINT_OUT_H__

#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

class PrintOut {
private:
	GLuint						mFontBase;
	int							mFontHeight, mFontWidth;
	char*						mFontName;

public:
	PrintOut(HDC hDC, char *fn, int fh = 18, int fw = 9);
	~PrintOut();

	void						Hint(int x, int y, const char *fmt, ...);

};

#endif ///> end of __PRINT_OUT_H__