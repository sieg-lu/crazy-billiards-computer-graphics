#ifndef __OBJECT_INTERFACE_H__
#define __OBJECT_INTERFACE_H__

#include <iostream>
#include <string>

class ObjectInterface {
protected:
	virtual bool			Drawing()												= 0;

public:
	virtual	bool 			Initialize()											= 0;
	virtual	bool 			Update()												= 0;
	virtual	bool 			Destroy()												= 0;

};

class BallInterface : public ObjectInterface {
protected:
	unsigned int			mTexture;
	std::string				mTextureFileName;

	float					mCoordX, mCoordY;
	float					mSpeedX, mSpeedY;
	float					mAccelX, mAccelY;
	float					mEpsX, mEpsY;

	float					mOmegaX, mOmegaY;
	float					mTheta;

	int						mHalfSideLength;

	bool					mDroppedDead;

	float*					mpFriction;

	int						mId;

public:
	virtual bool			Drawing()												= 0;

	virtual	bool 			Initialize()											= 0;
	virtual	bool 			Update()												= 0;
	virtual	bool 			Destroy()												= 0;

	bool					Stop()
	{
		mSpeedX = mSpeedY = 0.0f;
		mAccelX = mAccelY = 0.0f;
		return true;
	}

	virtual std::string		ClassName()												= 0;

	float*					getMpFriction() const									{ return mpFriction; }

	bool					IsDroppedDead() const									{ return mDroppedDead; }

	void					DebugOutput()
	{
		printf("CoordX: %.f, CoordY: %.f\n", mCoordX, mCoordY);
		printf("SpeedX: %.f, SpeedY: %.f\n", mSpeedX, mSpeedY);
		printf("AccelX: %.f, AccelY: %.f\n", mAccelX, mAccelY);
		printf("  EpsX: %.f,   EpsY: %.f\n", mEpsX, mEpsY);
		printf("-----------------------------------------------\n");
	}

	float 					getCoordX()	const										{ return mCoordX; }
	void					setCoordX(float val)									{ mCoordX = val; }
	float 					getCoordY() const										{ return mCoordY; }
	void					setCoordY(float val)									{ mCoordY = val; }

	int						getId() const											{ return mId; }

	void					setDroppedDead(bool val)								{ mDroppedDead = val; }
	
	friend class Collision;

};

#endif ///> end of __OBJECT_INTERFACE_H__