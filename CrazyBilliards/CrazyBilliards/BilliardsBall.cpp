#include <cmath>

#include "Utility.h"

#include "Global.h"
#include "BilliardsBall.h"

GeneralBall::GeneralBall(int id, float startX, float startY, char* textureFileName, float& pFric)
{
	mId = id;
	mpFriction = &pFric;
	mCoordX = startX;
	mCoordY = startY;
	mDroppedDead = false;
	mHalfSideLength = BALL_RADIUS;
	mTextureFileName = std::string(textureFileName);
}

bool GeneralBall::Drawing()
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(mCoordX - mHalfSideLength, mCoordY - mHalfSideLength);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(mCoordX + mHalfSideLength, mCoordY - mHalfSideLength);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(mCoordX + mHalfSideLength, mCoordY + mHalfSideLength);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(mCoordX - mHalfSideLength, mCoordY + mHalfSideLength);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	return true;
}

bool GeneralBall::Initialize()
{
	if (!Utility::LoadTexture(mTextureFileName.c_str(), mTexture)) {
		return false;
	}
	mSpeedX = mSpeedY = 0.0f;
	mAccelX = mAccelY = 0.0f;
	mRoundOver = true;

	return true;
}

bool GeneralBall::Update()
{
	if (mDroppedDead) {
		return false;
	}
	if (mCoordX + mSpeedX > BOARD_RIGHT_MOST || mCoordX + mSpeedX < BOARD_LEFT_MOST) {
		if (mCoordX + mSpeedX > BOARD_RIGHT_MOST) {
			mCoordX = BOARD_RIGHT_MOST;
		} else {
			mCoordX = BOARD_LEFT_MOST;
		}
		mSpeedX = -mSpeedX;
		mAccelX = -mAccelX;
	} else {
		mCoordX += mSpeedX;
		mSpeedX -= mAccelX;
	}
	if (mCoordY + mSpeedY > BOARD_BOTTOM_MOST || mCoordY + mSpeedY < BOARD_TOP_MOST) {
		if (mCoordY + mSpeedY > BOARD_BOTTOM_MOST) {
			mCoordY = BOARD_BOTTOM_MOST;
		} else {
			mCoordY = BOARD_TOP_MOST;
		}
		mSpeedY = -mSpeedY;
		mAccelY = -mAccelY;
	} else {
		mCoordY += mSpeedY;
		mSpeedY -= mAccelY;
	}

	if (GettingIntoHole()) {
		mDroppedDead = true;
	}

	if (abs(mSpeedX) < mEpsX || abs(mSpeedY) < mEpsY) {
		mSpeedX = 0.0f; mSpeedY = 0.0f;
		mAccelX = 0.0f; mAccelY = 0.0f;
		mRoundOver = true;
	}

	Drawing();

	return true;
}

bool GeneralBall::Destroy()
{
	glDeleteTextures(1, &mTexture);

	return true;
}

bool GeneralBall::GettingIntoHole()
{
	for (int i = 0; i < 4; i++) {
		if (Utility::FloatCompare(mCoordX, gsCornerHolePosX[i], CORENER_HOLE_IN_EPS) 
		 && Utility::FloatCompare(mCoordY, gsCornerHolePosY[i], CORENER_HOLE_IN_EPS)) {
			return true;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (Utility::IsCollide(mCoordX, mCoordY, gsMiddleHolePosX[i], gsMiddleHolePosY[i], gsMiddleHoleEps)) {
			return true;
		}
	}
	return false;
}

bool GeneralBall::HitTheBall(int forceX, int forceY, float hitDist, float hitPhi)
{
	if (Utility::FloatCompare(forceX, 0.0f, 1e-3) && Utility::FloatCompare(forceY, 0.0f, 1e-3)) {
		return false;
	}

	mSpeedX = (forceX * HIT_TIME / BALL_MASS);
	mAccelX = (forceX * ACCEL_RATIO * (*mpFriction));
	mEpsX = fabs(mAccelX);

	mSpeedY = (forceY * HIT_TIME / BALL_MASS);
	mAccelY = (forceY * ACCEL_RATIO * (*mpFriction));
	mEpsY = fabs(mAccelY);

	if (Utility::FloatCompare(forceX, 0.0f, 1e-3)) {
		mTheta = Utility::Degree2Radian(90) - atan((float)(forceX / forceY));
	} else {
		mTheta = atan((float)(forceY / forceX));
	}
	float K = mSpeedX / cos(mTheta);

	mOmegaX = 2.5f * K * hitDist * cos(hitPhi) / (BALL_RADIUS * BALL_RADIUS);
	mOmegaY = 2.5f * K * hitDist * sin(hitPhi) / (BALL_RADIUS * BALL_RADIUS);

	return true;
}

WhiteBall::WhiteBall(float startX, float startY, char* textureFileName, float& pFric)
	: GeneralBall(0, startX, startY, textureFileName, pFric)
{

}

bool WhiteBall::LeftButtonDown(int mouseX, int mouseY)
{
	float dist2 = (mouseX - mCoordX) * (mouseX - mCoordX) + (mouseY - mCoordY) * (mouseY - mCoordY);
	if (dist2 < BALL_RADIUS * BALL_RADIUS) {
		mIsSelected = true;
		mMouseX = mouseX;
		mMouseY = mouseY;

		return true;
	}
	return false;
}

bool WhiteBall::LeftButtonUp(int mouseX, int mouseY, float hitDist, float hitPhi)
{
	if (mIsSelected) {
		HitTheBall(mouseX - mMouseX, mouseY - mMouseY, hitDist, hitPhi);
		mIsFirstCollision = true;
	}
	mIsSelected = false;
	mMouseX = mMouseY = -1;
	return true;
}

bool WhiteBall::MouseMove(int mouseX, int mouseY)
{
	return true;
}

bool WhiteBall::Initialize()
{
	mIsSelected = false;
	mMouseX = mMouseY = -1;
	mEpsX = mEpsY = 0.0f;
	mIsFirstCollision = true;
	mOmegaX = mOmegaY = 0.0f;
	mTheta = 0.0f;
	mHitDistance = 0.0f;

	return GeneralBall::Initialize();
}

bool WhiteBall::Update()
{
	GeneralBall::Update();
	mOmegaX *= (float)pow((long double)e, (long double)-ALPHA);
	if (Utility::FloatCompare(mOmegaX, 0.0f, 1e-2)) {
		mOmegaX = 0.0f;
		mRoundOver &= true;
	}
	mOmegaY -= 2.5f * (*mpFriction) * 9.8f / (BALL_RADIUS * BALL_RADIUS);
	if (Utility::FloatCompare(mOmegaY, 0.0f, 1e-2)) {
		mOmegaY = 0.0f;
		mRoundOver &= true;
	}

	return true;
}

bool WhiteBall::Drawing()
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(mCoordX, mCoordY, 0);
	glRotatef(Utility::Radian2Degree(mOmegaX) * 2.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-mHalfSideLength, -mHalfSideLength);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(+mHalfSideLength, -mHalfSideLength);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(+mHalfSideLength, +mHalfSideLength);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-mHalfSideLength, +mHalfSideLength);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	return true;
}

ExplosionBall::ExplosionBall(float startX, float startY, float& pFric, std::vector<GeneralBall*>& genBalls)
	: GeneralBall(0, startX, startY, "Data/explode_ball.png", pFric)
{
	mpGeneralBallList = &genBalls;
}

bool ExplosionBall::Initialize()
{
	mIsIgnite = false;

	return GeneralBall::Initialize();;
}

bool ExplosionBall::Update()
{
	GeneralBall::Update();
	if (mIsIgnite && mRoundOver && !mDroppedDead) {
		BlastBalls(*mpGeneralBallList);
		setDroppedDead(true);
		return false;
	}

	return true;
}

void ExplosionBall::BlastBalls(std::vector<GeneralBall*> genBalls)
{
	for (int i = 0; i < genBalls.size(); i++) {
		float dist2 = 
			(mCoordX - genBalls[i]->getCoordX()) * (mCoordX - genBalls[i]->getCoordX())	+ 
			(mCoordY - genBalls[i]->getCoordY()) * (mCoordY - genBalls[i]->getCoordY());
		if (dist2 <= BLAST_RADIUS * BLAST_RADIUS) {
			float deltaX = genBalls[i]->getCoordX() - mCoordX;
			float deltaY = genBalls[i]->getCoordY() - mCoordY;
			genBalls[i]->HitTheBall(deltaX, deltaY, 0, 0);
		}
	}
}

FrictionBall::FrictionBall(float startX, float startY, float& pFric)
	: GeneralBall(0, startX, startY, "", pFric)
{
	srand((unsigned int)time(0));
	if (rand() & 1) {
		mTextureFileName = std::string("Data/ice_ball.png");
		mIsAddFriction = false;
	} else {
		mTextureFileName = std::string("Data/rough_ball.png");
		mIsAddFriction = true;
	}
}

bool FrictionBall::Initialize()
{
	mIsChanging = false;

	return GeneralBall::Initialize();
}

bool FrictionBall::Update()
{
	if (!GeneralBall::Update()) {
		return false;
	}
	if (mDroppedDead) {
		mIsChanging = true;
	}

	return true;
}
