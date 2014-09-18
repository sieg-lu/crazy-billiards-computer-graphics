#ifndef __HIGH_LEVEL_LOGIC_H__
#define __HIGH_LEVEL_LOGIC_H__

#include "Global.h"
#include "Utility.h"
#include "BilliardsBall.h"
#include "BilliardsBoard.h"
#include "Collision.h"
#include "HittingPoint.h"
#include "StrengthBar.h"
#include "PrintOut.h"
#include "Player.h"

#include <vector>

class GameLogicManager {
protected:
	HittingPoint*						mHitting;
	StrengthBar*						mStrength;
	BilliardsBoard*						mBoard;
	std::vector<GeneralBall*>			mGeneralBallList;
	WhiteBall*							mWhiteBall;
	std::vector<GeneralBall*>			mSpecialBallList;

//	FrictionBall*						mFricBall;

	PrintOut*							mHint;

	// true  - first player
	// false - second player
	bool								mRoundStart;
	bool								mIsFirstPlayer;

	Player*								mPlayer1;
	Player*								mPlayer2;

	bool								Holed(Player *p, bool correct, GeneralBall* ball);

public:
	virtual	bool 						Initialize(HDC hDC);
	virtual	bool 						Update();
	virtual	bool 						Destroy();

	WhiteBall*							getWhiteBall() const								{ return mWhiteBall; }
	BilliardsBoard*						getBoard() const									{ return mBoard; }
	HittingPoint*						getHitting() const									{ return mHitting; }
	StrengthBar*						getStrength() const									{ return mStrength; }

	void								ChangePlayer();

};


#endif ///> end of __HIGH_LEVEL_LOGIC_H__