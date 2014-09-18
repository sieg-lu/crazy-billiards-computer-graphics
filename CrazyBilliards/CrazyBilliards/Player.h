#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>

class Player {
private:
	std::vector<int>		mInBallIdList;

public:
	void					Holed(int id);
	bool					AllMyBallsHoled();

};


#endif ///> end of __PLAYER_H__