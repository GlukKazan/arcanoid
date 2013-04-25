#ifndef _WORLD_H_
#define _WORLD_H_

#include <set>
#include <Box2D.h>
#include "Desktop.h"
#include "Box2DItem.h"

const float W_WIDTH          = 10.0f;

const int   HALF_MARGIN      = 10;
const int   V_ITERATIONS     = 10;
const int   P_ITERATIONS     = 10;

const float GRAVITY          = 0.0f;
const float FRICTION         = 0.0f;
const float RESTITUTION      = 1.0f;
const float DYN_DENSITY      = 0.0f;

const float SPEED_EPS        = 1.0f;
const float BALL_SPEED       = 15.0f;
const float BULLET_SPEED     = 10.0f;
const float GIFT_SPEED       = 5.0f;

using namespace std;

class World: public b2ContactListener {
	private:
		bool                   isFired;
		b2World*               wp;
		set<b2Body*>*          bodies;
	    virtual void           PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
		b2Body*                createBox(int x, int y, int hw, int hh);
		b2Body*                createPlatform(int x, int y, int hw, int hh);
		b2Body*                createBall(int x, int y, int r);
		b2Body*                createBullet(int x, int y, int hw, int hh);
		b2Body*                createGift(int x, int y, int r);
		float32                getTimeStep();
		void                   normalize(b2Vec2& v);
		float32                sign(float32 x);
	public:
		World(): wp(NULL), bodies(NULL) {}

		void                   init();
		void                   release();
		void                   update();

		static  float          toWorld(int x) {return ((float)x * W_WIDTH) / (float)desktop.getWidth();}
		static  int            fromWorld(float x) {return (int)((x * (float)desktop.getWidth()) / W_WIDTH);}

		void                   setFired() {isFired = true;}
		void                   dropBody(b2Body* b);
		b2Body*                createBody(Box2DItem* it);
		void                   normalizeSpeed(b2Vec2& v);

	typedef set<b2Body*>::iterator BIter;
};

extern World world;

#endif	// _WORLD_H_
