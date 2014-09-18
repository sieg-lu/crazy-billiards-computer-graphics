#include "Player.h"

void Player::Holed(int id)
{
	mInBallIdList.push_back(id);
}

bool Player::AllMyBallsHoled()
{
	bool flag[8], res = true;
	bool eight = false;
	memset(flag, 0, sizeof(flag));
	for (int i = 0; i < mInBallIdList.size(); i++) {
		flag[mInBallIdList[i] % 8] = true;
		if (mInBallIdList[i] == 8) {
			eight = true;
		}
	}
	for (int i = 1; i < 8; i++) {
		res &= flag[i];
	}

	return res & eight;
}

