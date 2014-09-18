#ifndef __BILLIARDS_BALL_H__
#define __BILLIARDS_BALL_H__

#include "ObjectInterface.h"
#include <vector>

class GeneralBall : public BallInterface {
protected:
	virtual bool			Drawing();
	bool					GettingIntoHole();

	// Only used on WhiteBall;
	bool					mRoundOver;

public:
	GeneralBall(int id, float startX, float startY, char* textureFileName, float& pFric);

	virtual	bool 			Initialize();
	virtual	bool 			Update();
	virtual	bool 			Destroy();

	virtual bool			HitTheBall(int forceX, int forceY, float hitDist, float hitPhi);

	virtual std::string		ClassName()											{ return std::string("GeneralBall"); }

	bool					getRoundOver() const								{ return mRoundOver; }
	void					setRoundOver(bool val)								{ mRoundOver = val; }

};

class WhiteBall : public GeneralBall {
protected:
	bool					mIsSelected;
	int						mMouseX, mMouseY;

	bool					mIsFirstCollision;
	float					mHitDistance;

	virtual bool			Drawing();

public:
	WhiteBall(float startX, float startY, char* textureFileName, float& pFric);

	virtual	bool 			Initialize();
	virtual bool			Update();

	bool					LeftButtonDown(int mouseX, int mouseY);
	bool					LeftButtonUp(int mouseX, int mouseY, float hitDist, float hitPhi);
	bool					MouseMove(int mouseX, int mouseY);

	virtual std::string		ClassName()											{ return std::string("WhiteBall"); }

	bool					getIsFirstCollision() const							{ return mIsFirstCollision; }
	void					setIsFirstCollision(bool val)						{ mIsFirstCollision = val; }
	float					getOmegaY() const									{ return mOmegaY; }
	void					setOmegaY(float val)								{ mOmegaY = val; }

	float					getTheta() const									{ return mTheta; }
	bool					IsSelected() const									{ return mIsSelected; }

};

class ExplosionBall : public GeneralBall {
protected:
	bool					mIsIgnite;

	std::vector<GeneralBall*>*
							mpGeneralBallList;

public:
	ExplosionBall(float startX, float startY, float& pFric, std::vector<GeneralBall*>& genBalls);

	virtual	bool 			Initialize();
 	virtual	bool 			Update();
// 	virtual	bool 			Destroy();

	virtual std::string		ClassName()											{ return std::string("ExplosionBall"); }

	void					BlastBalls(std::vector<GeneralBall*> genBalls);

	bool					getIsIgnite() const									{ return mIsIgnite; }
	void					setIsIgnite(bool val)								{ mIsIgnite = val; }

};

class FrictionBall : public GeneralBall {
protected:
	bool					mIsAddFriction;
	bool					mIsChanging;
public:
	FrictionBall(float startX, float startY, float& pFric);

 	virtual	bool 			Initialize();
 	virtual	bool 			Update();
// 	virtual	bool 			Destroy();

	virtual std::string		ClassName()											{ return std::string("FrictionBall"); }

	bool					getIsAddFriction() const							{ return mIsAddFriction; }
	void					setIsAddFriction(bool val)							{ mIsAddFriction = val; }
	bool					getIsChanging() const								{ return mIsChanging; }
	void					setIsChanging(bool val)								{ mIsChanging = val; }

};


#endif ///> end of __BILLIARDS_BALL_H__