#include "HittingPoint.h"

#include "Global.h"
#include "Utility.h"

#define HITTING_HALF_LENGTH		70
#define HITTING_BALL_RADIUS		HITTING_HALF_LENGTH * 4 / 7
#define DOT_HALF_LENGTH			4

bool HittingPoint::Initialize()
{
	if (!Utility::LoadTexture("Data/hitting.png", mBallTexture)
	 || !Utility::LoadTexture("Data/reddot.png", mDotTexture)) {
		 return false;
	}
	mCoordX = 100;
	mCoordY = 100;
	mDotX = mDotY = 100;
	mIsSelected = false;
	mMouseX = mMouseY = -1;

	return true;
}

bool HittingPoint::Update()
{
	Drawing();

	return true;
}

bool HittingPoint::Destroy()
{
	glDeleteTextures(1, &mBallTexture);
	glDeleteTextures(1, &mDotTexture);

	return true;
}

bool HittingPoint::Drawing()
{
	glBindTexture(GL_TEXTURE_2D, mBallTexture);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslated(mCoordX, mCoordY, 0);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-HITTING_HALF_LENGTH, -HITTING_HALF_LENGTH);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(+HITTING_HALF_LENGTH, -HITTING_HALF_LENGTH);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(+HITTING_HALF_LENGTH, +HITTING_HALF_LENGTH);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-HITTING_HALF_LENGTH, +HITTING_HALF_LENGTH);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mDotTexture);
	glTranslated(mDotX, mDotY, 0);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-DOT_HALF_LENGTH, -DOT_HALF_LENGTH);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(+DOT_HALF_LENGTH, -DOT_HALF_LENGTH);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(+DOT_HALF_LENGTH, +DOT_HALF_LENGTH);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-DOT_HALF_LENGTH, +DOT_HALF_LENGTH);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	return true;
}

bool HittingPoint::LeftButtonDown(int mouseX, int mouseY)
{
	float dist2 = (mouseX - mDotX) * (mouseX - mDotX) + (mouseY - mDotY) * (mouseY - mDotY);
	if (dist2 < DOT_HALF_LENGTH * DOT_HALF_LENGTH) {
		mIsSelected = true;
		mMouseX = mouseX;
		mMouseY = mouseY;

		return true;
	}
	return false;
}

bool HittingPoint::LeftButtonUp(int mouseX, int mouseY)
{
	mIsSelected = false;
	mMouseX = mMouseY = -1;
	return true;
}

bool HittingPoint::MouseMove(int mouseX, int mouseY)
{
	float dist2 = (mouseX - mCoordX) * (mouseX - mCoordX) + (mouseY - mCoordY) * (mouseY - mCoordY);
	if (mIsSelected && dist2 < HITTING_BALL_RADIUS * HITTING_BALL_RADIUS) {
		mDotX = mouseX - mMouseX + mDotX;
		mDotY = mouseY - mMouseY + mDotY;
		mMouseX = mouseX;
		mMouseY = mouseY;
	}
	return true;
}

float HittingPoint::GetHittingDist()
{
	return sqrt((float)((mDotX - mCoordX) * (mDotX - mCoordX) + (mDotY - mCoordY) * (mDotY - mCoordY)));
}

float HittingPoint::GetHittingPhi()
{
	if (Utility::FloatCompare(mDotX, mCoordX, 1e-2)) {
		return 0;
	}
	return atan((float)((mDotX - mCoordY) / (mDotX - mCoordX)));
}
