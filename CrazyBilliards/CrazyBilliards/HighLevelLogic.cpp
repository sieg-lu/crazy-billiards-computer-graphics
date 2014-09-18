#include "HighLevelLogic.h"

bool GameLogicManager::Initialize(HDC hDC)
{
	mHint = new PrintOut(hDC, "Arial");

	mPlayer1 = new Player();
	mPlayer2 = new Player();

	mBoard = new BilliardsBoard();
	if (!mBoard->Initialize()) {
		return false;
	}

	mHitting = new HittingPoint();
	if (!mHitting->Initialize()) {
		return false;
	}

	mStrength = new StrengthBar();
	if (!mStrength->Initialize()) {
		return false;
	}

	for (int i = 0; i < 15; i++) {
		char buf[100];
		wsprintf(buf, "Data/%d.png", i + 1);
		GeneralBall* ball = 
			new GeneralBall(i + 1, gsRelativeStartPosX[i] + (int)(0.65f * (float)WINDOW_WIDTH), 
			gsRelativeStartPosY[i] + WINDOW_HALF_HEIGHT, 
			buf, mBoard->mFriction);
		if (!ball->Initialize()) {
			return false;
		}
		mGeneralBallList.push_back(ball);
	}

	mWhiteBall = 
		new WhiteBall((int)(0.35f * (float)WINDOW_WIDTH), 
		WINDOW_HALF_HEIGHT, "Data/saber1.png", 
		mBoard->mFriction);
	if (!mWhiteBall->Initialize()) {
		return false;
	}

	mIsFirstPlayer = true;
	mRoundStart = false;

	return true;
}

bool GameLogicManager::Update()
{
	mBoard->Update();
	mHitting->Update();
	mStrength->Update();
// 	for (int i = 0; i < 6; i++) {
// 		gGenBall[i]->Update();
// 	}
	for (int i = 0; i < mGeneralBallList.size(); i++) {
		if (mGeneralBallList[i]->Update() && mGeneralBallList[i]->IsDroppedDead()) {
			if (mIsFirstPlayer) {
				int id = mGeneralBallList[i]->getId();
				if (id >= 9 && id < 16) {
					Holed(mPlayer1, true, mGeneralBallList[i]);
				} else {
					Holed(mPlayer1, false, mGeneralBallList[i]);
				}
			} else {
				int id = mGeneralBallList[i]->getId();
				if (id >= 1 && id < 8) {
					Holed(mPlayer2, true, mGeneralBallList[i]);
				} else {
					Holed(mPlayer2, false, mGeneralBallList[i]);
				}
			}
		}
	}
	mWhiteBall->Update();
	for (int i = 0; i < mSpecialBallList.size(); i++) {
		mSpecialBallList[i]->Update();
	}

	if (mPlayer1->AllMyBallsHoled()) {
		mHint->Hint(300, 300, "Player1 Wins!");
	}
	if (mPlayer2->AllMyBallsHoled()) {
		mHint->Hint(300, 400, "Player2 Wins!");
	}

	// Collision
	for (int i = 0; i < mSpecialBallList.size(); i++) {
		if (Collision::DoCollide(mSpecialBallList[i], mWhiteBall)) {
			if (mSpecialBallList[i]->ClassName() == std::string("ExplosionBall")) {
				ExplosionBall* exBall = static_cast<ExplosionBall*>(mSpecialBallList[i]);
				exBall->setIsIgnite(true);
				exBall->setRoundOver(false);
			}
		}
	}
	for (int i = 0; i < mSpecialBallList.size(); i++) {
		if (mSpecialBallList[i]->ClassName() == std::string("FrictionBall")) {
			FrictionBall* friBall = static_cast<FrictionBall*>(mSpecialBallList[i]);
			if (friBall->getIsChanging()) {
				friBall->setIsChanging(false);
				if (friBall->getIsAddFriction()) {
					mBoard->setFriction(5.0f);
				} else {
					mBoard->setFriction(0.5f);
				}
			}
		}
	}

	for (int i = 0; i < mGeneralBallList.size(); i++) {
		Collision::DoCollide(mWhiteBall, mGeneralBallList[i]);
	}

	for (int i = 0; i < mGeneralBallList.size(); i++) {
		for (int j = i + 1; j < mGeneralBallList.size(); j++) {
			Collision::DoCollide(mGeneralBallList[i], mGeneralBallList[j]);
		}
	}

	for (int i = 0; i < mSpecialBallList.size(); i++) {
		for (int j = i + 1; j < mSpecialBallList.size(); j++) {
			Collision::DoCollide(mSpecialBallList[i], mSpecialBallList[j]);
		}
	}

	for (int i = 0; i < mGeneralBallList.size(); i++) {
		for (int j = 0; j < mSpecialBallList.size(); j++) {
			Collision::DoCollide(mGeneralBallList[i], mSpecialBallList[j]);
		}
	}

	mHint->Hint(500, 500, "Player: %s", (mIsFirstPlayer ? "1-7" : "9-15"));

	return true;
}

bool GameLogicManager::Destroy()
{
	mBoard->Destroy();
	delete mBoard;

	mHitting->Destroy();
	delete mHitting;

	mStrength->Destroy();
	delete mStrength;

// 	for (int i = 0; i < 6; i++) {
// 		gGenBall[i]->Destroy();
// 		delete gGenBall[i];
// 	}
// 	delete[] gGenBall;
	for (int i = 0; i < mGeneralBallList.size(); i++) {
		mGeneralBallList[i]->Destroy();
		delete mGeneralBallList[i];
	}
	mGeneralBallList.clear();

	for (int i = 0; i < mSpecialBallList.size(); i++) {
		mSpecialBallList[i]->Destroy();
		delete mSpecialBallList[i];
	}
	mSpecialBallList.clear();

	mWhiteBall->Destroy();
	delete mWhiteBall;

	delete mPlayer1;
	delete mPlayer2;

	delete mHint;

	return true;
}

void GameLogicManager::ChangePlayer()
{
	mIsFirstPlayer = !mIsFirstPlayer;
}

bool GameLogicManager::Holed(Player* p, bool correct, GeneralBall* ball)
{
	if (correct) {
		p->Holed(ball->getId());
		ChangePlayer();

		srand((unsigned int)time(0));
		GeneralBall* specialBall;
		int xPos = rand() % BOARD_WIDTH + BOARD_LEFT_MOST;
		int yPos = rand() % BOARD_HEIGHT + BOARD_TOP_MOST;

		if (rand() & 1) {
			specialBall = new ExplosionBall(xPos, yPos, mBoard->mFriction, mGeneralBallList);
			specialBall->Initialize();
		} else {
			specialBall = new FrictionBall(xPos, yPos, mBoard->mFriction);
			specialBall->Initialize();
		}
		mSpecialBallList.push_back(specialBall);

		mBoard->setFriction(1.0f);

		return true;
	}

	ball->setDroppedDead(false);
	ball->setCoordX(gsRelativeStartPosX[ball->getId()] + (int)(0.65f * (float)WINDOW_WIDTH));
	ball->setCoordY(gsRelativeStartPosY[ball->getId()] + WINDOW_HALF_HEIGHT);
	ball->Stop();
	
	return false;
}

