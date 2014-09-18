#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Global.h"
#include "ObjectInterface.h"

class Collision {
private:

public:
	static bool				IsCollide(BallInterface* ith, BallInterface* jth, float dist = COLLISION_EPS);
	static bool				DoCollide(BallInterface* ith, BallInterface* jth);
	static bool				DragOut(BallInterface* ith, BallInterface* jth);

};


#endif ///> end of __COLLISION_H__