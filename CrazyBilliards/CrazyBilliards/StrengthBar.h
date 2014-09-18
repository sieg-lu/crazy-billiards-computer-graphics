#ifndef __STRENGTH_BAR_H__
#define __STRENGTH_BAR_H__

#include "ObjectInterface.h"

#define STRENGTH_HALF_WIDTH			25
#define STRENGTH_HALF_HEIGHT		100

class StrengthBar : public ObjectInterface {
protected:
	GLuint					mTexture;
	int						mCoordX, mCoordY;
	float					mStrength;
	// Invarient: 0 <= mStrenth <= 100;
	bool					mIsSelected;

	virtual bool			Drawing();

public:
	virtual	bool 			Initialize();
	virtual	bool 			Update();
	virtual	bool 			Destroy();

	bool					LeftButtonDown(bool isWhiteBallSelected);
	bool					LeftButtonUp();
	bool					MouseMove(int forceX, int forceY);

};


#endif ///> end of __STRENGTH_BAR_H__