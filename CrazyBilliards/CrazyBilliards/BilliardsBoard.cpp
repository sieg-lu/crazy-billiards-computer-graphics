#include "Utility.h"

#include "Global.h"
#include "BilliardsBoard.h"

bool BilliardsBoard::Initialize()
{
	if (!Utility::LoadTexture("Data/billiards_board.png", mTexture[NORMAL_BOARD])
	 || !Utility::LoadTexture("Data/billiards_board_rough.png", mTexture[ROUGH_BOARD])
	 || !Utility::LoadTexture("Data/billiards_board_smooth.png", mTexture[SMOOTH_BOARD])) {
		return false;
	}
	mFriction = 1.0f;

	return true;
}

bool BilliardsBoard::Update()
{
	Drawing();

	return true;
}

bool BilliardsBoard::Destroy()
{
	for (int i = 0; i < 3; i++) {
		glDeleteTextures(1, &mTexture[i]);
	}

	return true;
}

bool BilliardsBoard::Drawing()
{
	if (mFriction == 1.0f) {
		glBindTexture(GL_TEXTURE_2D, mTexture[NORMAL_BOARD]);
	} else if (mFriction < 1.0f) {
		glBindTexture(GL_TEXTURE_2D, mTexture[SMOOTH_BOARD]);
	} else {
		glBindTexture(GL_TEXTURE_2D, mTexture[ROUGH_BOARD]);
	}
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f((WINDOW_WIDTH - BOARD_WIDTH) / 2, (WINDOW_HEIGHT - BOARD_HEIGHT) / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex2f((WINDOW_WIDTH + BOARD_WIDTH) / 2, (WINDOW_HEIGHT - BOARD_HEIGHT) / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex2f((WINDOW_WIDTH + BOARD_WIDTH) / 2, (WINDOW_HEIGHT + BOARD_HEIGHT) / 2);
		glTexCoord2f(0.0f, 1.0f); glVertex2f((WINDOW_WIDTH - BOARD_WIDTH) / 2, (WINDOW_HEIGHT + BOARD_HEIGHT) / 2);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	return true;
}
