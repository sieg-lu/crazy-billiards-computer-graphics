#ifndef __HITTING_POINT_H__
#define __HITTING_POINT_H__

#include "Global.h"
#include "ObjectInterface.h"

class HittingPoint : public ObjectInterface {
protected:
	GLuint					mBallTexture;
	GLuint					mDotTexture;

	int						mCoordX, mCoordY;
	// Notice: mDotX, mDotY are relative to mCoordX and mCoordY
	int						mDotX, mDotY;
	int						mMouseX, mMouseY;
	bool					mIsSelected;

	virtual bool			Drawing();

public:
	virtual	bool 			Initialize();
	virtual	bool 			Update();
	virtual	bool 			Destroy();

	bool					LeftButtonDown(int mouseX, int mouseY);
	bool					LeftButtonUp(int mouseX, int mouseY);
	bool					MouseMove(int mouseX, int mouseY);

	float					GetHittingDist();
	float					GetHittingPhi();
};


#endif ///> end of __HITTING_POINT_H__