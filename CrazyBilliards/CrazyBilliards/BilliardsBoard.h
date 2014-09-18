#ifndef __BILLIARDS_BOARD_H__
#define __BILLIARDS_BOARD_H__

#include "ObjectInterface.h"

#define NORMAL_BOARD		0
#define ROUGH_BOARD			1
#define SMOOTH_BOARD		2

class BilliardsBoard : public ObjectInterface {
protected:
	GLuint					mTexture[3];
	float					mFriction;

	virtual	bool			Drawing();

public:
	virtual	bool 			Initialize();
	virtual	bool 			Update();
	virtual	bool 			Destroy();

	float					getFriction() const							{ return mFriction; }
	void					setFriction(float val)						{ mFriction = val; }

	friend class			GameLogicManager;

};

#endif ///> end of __BILLIARDS_BOARD_H__