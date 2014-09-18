#include "PrintOut.h"


PrintOut::PrintOut(HDC hDC, char *fn, int fh /*= 18*/, int fw /*= 9*/)
{
	HFONT font;
	HFONT oldfont;

	mFontBase = glGenLists(96);

	font = CreateFont(
		-fh,							// Height Of Font
		fw,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		TRUE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		fn);							// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, mFontBase);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

PrintOut::~PrintOut()
{
	glDeleteLists(mFontBase, 96);
}

void PrintOut::Hint(int x, int y, const char *fmt, ...)
{
	char text[256];
	va_list ap;

	if(fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	glRasterPos2f(x, y);
	glPushAttrib(GL_LIST_BIT);
	glListBase(mFontBase - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
