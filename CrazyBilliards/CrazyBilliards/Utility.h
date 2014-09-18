#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <iostream>
#include <stdarg.h>

#include "libpng/png.h"

#include "Global.h"

struct glTextureT {
	GLsizei								width;
	GLsizei								height;
	GLenum								format;
	GLint								internalFormat;
	GLuint								id;
	GLubyte*							texels;
};

class Utility {
private:
	static void							GetPngTextureInfo(int color_type, glTextureT *texinfo);
	static glTextureT*					LoadPngTexture(const char* filename);
	static AUX_RGBImageRec*				LoadBmpTexture(char* filename);
	template<class T> static bool		TCompare(T a, T b, T eps)										{ return fabs(a - b) < eps; }

public:
	static bool							LoadTexture(const char* filename, GLuint& texture);
	static bool							FloatCompare(float a, float b, float eps = COLLISION_EPS);
	static bool							IsCollide(float ax, float ay, float bx, float by, float len);

	static float						Degree2Radian(float deg);
	static float						Radian2Degree(float rad);

};

#endif ///> end of __UTILITY_H__