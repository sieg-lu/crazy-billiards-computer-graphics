#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define WINDOW_WIDTH						1400
#define WINDOW_HALF_WIDTH					WINDOW_WIDTH / 2
#define WINDOW_HEIGHT						600
#define WINDOW_HALF_HEIGHT					WINDOW_HEIGHT / 2

#define FPS									60

#define BOARD_WIDTH							976
#define BOARD_HEIGHT						588
#define BOARD_LEFT_MOST						(WINDOW_WIDTH - BOARD_WIDTH) / 2 + 67
#define BOARD_RIGHT_MOST					(WINDOW_WIDTH + BOARD_WIDTH) / 2 - 67
#define BOARD_TOP_MOST						(WINDOW_HEIGHT - BOARD_HEIGHT) / 2 + 67
#define BOARD_BOTTOM_MOST					(WINDOW_HEIGHT + BOARD_HEIGHT) / 2 - 67

#define BALL_RADIUS							16
#define HOLE_RADIUS							16
#define BALL_MASS							75.0f
#define ACCEL_RATIO							51.0f / 18000.0f

#define BLAST_RADIUS						150.0f

#define ALPHA								0.01f
#define e									2.7183f
#define PI									3.1416f
#define OMEGA_K								0.01f

#define MAXIMUM_FORCE						200.0f

#define COLLISION_EPS						(BALL_RADIUS + BALL_RADIUS) * 0.95
#define MIDDLE_HOLE_IN_EPS					(BALL_RADIUS + HOLE_RADIUS)
#define CORENER_HOLE_IN_EPS					(BALL_RADIUS + HOLE_RADIUS) * 0.8f

#define HIT_TIME							12.0f

#define KINETIC_LOSS_A						0.80f
#define KINETIC_LOSS_B						0.90f

static const float gsMiddleHoleEps =		sqrt(0.1f * HOLE_RADIUS * HOLE_RADIUS + 0.2f * (HOLE_RADIUS + BALL_RADIUS) + BALL_RADIUS * BALL_RADIUS);

static const int gsCornerHolePosX[4] =		{ WINDOW_HALF_WIDTH - 440, WINDOW_HALF_WIDTH + 440, WINDOW_HALF_WIDTH - 440, WINDOW_HALF_WIDTH + 440 };
static const int gsCornerHolePosY[4] =		{ WINDOW_HALF_HEIGHT + 245, WINDOW_HALF_HEIGHT + 245, WINDOW_HALF_HEIGHT - 245, WINDOW_HALF_HEIGHT - 245 };
static const int gsMiddleHolePosX[2] =		{ WINDOW_HALF_WIDTH, WINDOW_HALF_WIDTH };
static const int gsMiddleHolePosY[2] =		{ WINDOW_HALF_HEIGHT + 240, WINDOW_HALF_HEIGHT - 240 };

static const float gsRelativeStartPosX[16] =
{
	0, 1.7321f * BALL_RADIUS, 1.7321f * BALL_RADIUS, 3.4641f * BALL_RADIUS,
	3.4641f * BALL_RADIUS, 3.4641f * BALL_RADIUS, 5.1962f * BALL_RADIUS, 
	5.1962f * BALL_RADIUS, 5.1962f * BALL_RADIUS, 5.1962f * BALL_RADIUS, 6.9282f * BALL_RADIUS, 
	6.9282f * BALL_RADIUS, 6.9282f * BALL_RADIUS, 6.9282f * BALL_RADIUS, 6.9282f * BALL_RADIUS
};
static const int gsRelativeStartPosY[16] =
{
	0, BALL_RADIUS, -BALL_RADIUS, 2.0f * BALL_RADIUS,
	0, -2.0f * BALL_RADIUS, 3.0f * BALL_RADIUS, BALL_RADIUS,
	-BALL_RADIUS, -3.0f * BALL_RADIUS, 4.0f * BALL_RADIUS, 2.0f * BALL_RADIUS,
	0, -2.0f * BALL_RADIUS, -4.0f * BALL_RADIUS
};

static int gFps =							60;

#endif ///> end of __GLOBAL_H__