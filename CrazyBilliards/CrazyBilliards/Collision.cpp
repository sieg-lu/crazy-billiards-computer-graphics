#include "Collision.h"
#include "BilliardsBall.h"
#include "Utility.h"
#include <cmath>



bool Collision::DoCollide(BallInterface* ith, BallInterface* jth)
{
	if (!IsCollide(ith, jth)) {
		return false;
	}

	float Xi = ith->mCoordX, Yi = ith->mCoordY;
	float Xj = jth->mCoordX, Yj = jth->mCoordY;
	float Vix = ith->mSpeedX, Viy = ith->mSpeedY;
	float Vjx = jth->mSpeedX, Vjy = jth->mSpeedY;
	float res = 0;

	float deltaX = Xj - Xi;
	float deltaY = Yj - Yi;
	float plusDx2Dy2 = deltaX * deltaX + deltaY * deltaY;

	float Unx = (Xj - Xi) / sqrt(plusDx2Dy2);
	float Uny = (Yj - Yi) / sqrt(plusDx2Dy2);

	float Vni = Vix * Unx + Viy * Uny;
	float Vnj = Vjx * Unx + Vjy * Uny;
	float Vti = Vix * (-Uny) + Viy * Unx;
	float Vtj = Vjx * (-Uny) + Vjy * Unx;

	float VixP = Vnj * Unx - Uny * Vti;
	float ViyP = Vnj * Uny + Unx * Vti;
	float VjxP = Vni * Unx - Uny * Vtj;
	float VjyP = Vni * Uny + Unx * Vtj;

	if (ith->ClassName() == std::string("WhiteBall")) {
		WhiteBall* wb = static_cast<WhiteBall*>(ith);
		if (wb->getIsFirstCollision()) {
			wb->setIsFirstCollision(false);
			wb->setOmegaY(0.0f);
			float tao = 0.5f;
			float Vrw = sqrt(0.4f * BALL_RADIUS * BALL_RADIUS / tao) * wb->getOmegaY();
			VixP += Vrw * cos(wb->getTheta());
			ViyP += Vrw * sin(wb->getTheta());
		}
	}

	ith->mSpeedX = VixP * KINETIC_LOSS_A;
	ith->mAccelX = ith->mSpeedX * BALL_MASS / HIT_TIME * ACCEL_RATIO * *(ith->mpFriction);
	ith->mEpsX = fabs(ith->mAccelX);

	ith->mSpeedY = ViyP * KINETIC_LOSS_A;
	ith->mAccelY = ith->mSpeedY * BALL_MASS / HIT_TIME * ACCEL_RATIO * *(ith->mpFriction);
	ith->mEpsY = fabs(ith->mAccelY);

	jth->mSpeedX = VjxP * KINETIC_LOSS_B;
	jth->mAccelX = jth->mSpeedX * BALL_MASS / HIT_TIME * ACCEL_RATIO * *(ith->mpFriction);
	jth->mEpsX = fabs(jth->mAccelX);

	jth->mSpeedY = VjyP * KINETIC_LOSS_B;
	jth->mAccelY = jth->mSpeedY * BALL_MASS / HIT_TIME * ACCEL_RATIO * *(ith->mpFriction);
	jth->mEpsY = fabs(jth->mAccelY);

	DragOut(ith, jth);

	return true;
}

bool Collision::IsCollide(BallInterface* ith, BallInterface* jth, float dist /*= COLLISION_EPS*/)
{
	float deltaX = jth->mCoordX - ith->mCoordX;
	float deltaY = jth->mCoordY - ith->mCoordY;
	float plusDx2Dy2 = deltaX * deltaX + deltaY * deltaY;

	return plusDx2Dy2 <= dist * dist;
}

bool Collision::DragOut(BallInterface* ith, BallInterface* jth)
{
	if (!IsCollide(ith, jth)) {
		return false;
	}

	float Xi = ith->mCoordX, Yi = ith->mCoordY;
	float Xj = jth->mCoordX, Yj = jth->mCoordY;

	float deltaX = Xj - Xi;
	float deltaY = Yj - Yi;
	float plusDx2Dy2 = deltaX * deltaX + deltaY * deltaY;

	if (plusDx2Dy2 < 4.0f * BALL_RADIUS * BALL_RADIUS) {
		float halfDragDist = (BALL_RADIUS - sqrt(plusDx2Dy2) / 2.0f);

		float deltaSignX, deltaSignY; 
		if (Utility::FloatCompare(Xi, Xj, 1e-3)) {
			deltaSignX = 0;
		} else {
			deltaSignX = (Xi - Xj) / abs(Xi - Xj);
		}
		if (Utility::FloatCompare(Yi, Yj, 1e-3)) {
			deltaSignY = 0;
		} else {
			deltaSignY = (Yi - Yj) / abs(Yi - Yj);
		}

		ith->setCoordX(ith->getCoordX() + deltaSignX * halfDragDist);
		ith->setCoordY(ith->getCoordY() + deltaSignY * halfDragDist);
		jth->setCoordX(jth->getCoordX() - deltaSignX * halfDragDist);
		jth->setCoordY(jth->getCoordY() - deltaSignY * halfDragDist);

		return true;
	}

	return false;
}

