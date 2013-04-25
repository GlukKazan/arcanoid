#ifndef _BALL_H_
#define _BALL_H_

#include <vector>
#include <string>
#include "s3e.h"

#include "Desktop.h"
#include "World.h"
#include "Box2DItem.h"

#define BALL_RADIUS        10
#define INIT_SPEED_X      -5.0f
#define INIT_SPEED_Y      -15.0f

using namespace std;

enum EBallType {
	ebtNormal            = 0x00,
	ebtFire              = 0x01,
	ebtBomb              = 0x02
};

class Ball: public Box2DItem {
    private:
        int      L, T, R, B;
		string*  img;
		int      weight;
		bool     isContacted;
		float32  deltaAngle;
    public:
		Ball(int x, int y, int s);
		Ball(const Ball& p);
        void        release();
        void        refresh();
		void        setImg(int tp, int w, const char* res, int r, int ll = 0, int tt = 0, int rr = 0, int bb = 0);
		void        getPosition();
		bool        isOut() const;
		virtual int getWeight() {return weight;}

	friend class Balls;
};

#endif	// _BALL_H_
