#include "Utility.h"
#include "Global.h"

#include "StrengthBar.h"


bool StrengthBar::Drawing()
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glPushMatrix();
	glTranslated(mCoordX, mCoordY, 0);
	glScalef(1.0f, mStrength / 100.0f, 1.0f);

// 	glDisable(GL_TEXTURE_2D);
// 	glColor3f(0.0f, 1.0f, 1.0f);
// 	glBegin(GL_QUADS);
// 	{
// 		glNormal3f(0.0f, 0.0f, 1.0f);
// 		glVertex2f(-STRENGTH_HALF_WIDTH, -STRENGTH_HALF_HEIGHT);
// 		glVertex2f(+STRENGTH_HALF_WIDTH, -STRENGTH_HALF_HEIGHT);
// 		glVertex2f(+STRENGTH_HALF_WIDTH, +STRENGTH_HALF_HEIGHT);
// 		glVertex2f(-STRENGTH_HALF_WIDTH, +STRENGTH_HALF_HEIGHT);
// 	}
// 	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-STRENGTH_HALF_WIDTH, -STRENGTH_HALF_HEIGHT);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(+STRENGTH_HALF_WIDTH, -STRENGTH_HALF_HEIGHT);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(+STRENGTH_HALF_WIDTH, +STRENGTH_HALF_HEIGHT);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-STRENGTH_HALF_WIDTH, +STRENGTH_HALF_HEIGHT);
	}
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslated(mCoordX, mCoordY, 0);
	glLineWidth(3.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(-STRENGTH_HALF_WIDTH, -STRENGTH_HALF_HEIGHT);
		glVertex2f(+STRENGTH_HALF_WIDTH, -STRENGTH_HALF_HEIGHT);
		glVertex2f(+STRENGTH_HALF_WIDTH, +STRENGTH_HALF_HEIGHT);
		glVertex2f(-STRENGTH_HALF_WIDTH, +STRENGTH_HALF_HEIGHT);
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glPopMatrix();

	return true;
}

bool StrengthBar::Initialize()
{
	if (!Utility::LoadTexture("Data/StrengthBar.png", mTexture)) {
		return false;
	}
	mCoordX = 100;
	mCoordY = 300;
	mStrength = 0.0f;
	mIsSelected = false;

	return true;
}

bool StrengthBar::Update()
{
	Drawing();

	return true;
}

bool StrengthBar::Destroy()
{
	glDeleteTextures(1, &mTexture);

	return true;
}

bool StrengthBar::LeftButtonDown(bool isWhiteBallSelected)
{
	mIsSelected = isWhiteBallSelected;

	return true;
}

bool StrengthBar::LeftButtonUp()
{
	mStrength = 0.0f;

	return true;
}

bool StrengthBar::MouseMove(int forceX, int forceY)
{
	float forceLen = sqrt((float)(forceX * forceX + forceY * forceY));
	if (mIsSelected && (forceLen >= 0.0f && forceLen <= 100.0f)) {
		mStrength = forceLen;
	}

	return true;
}
