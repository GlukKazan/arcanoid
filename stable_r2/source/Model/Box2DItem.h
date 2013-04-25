#ifndef _BOX2D_ITEM_H_
#define _BOX2D_ITEM_H_

#include <Box2D.h>

enum EItemType {
	eitUnknown               = 0,
	eitPlatform              = 1,
	eitBall                  = 2,
	eitBrick                 = 3,
	eitFire                  = 4,
	eitGift                  = 5
};

enum ELifeStatus {
	elsCreating              = 1,
	elsNornmal               = 2,
	elsDeleting              = 3,
	elsDeleted               = 4,
	elsDisabling             = 5,
	elsDisabled              = 6
};

class Box2DItem {
	protected:
		int                    type;
		int                    status;
		int                    flags;
		int                    X, Y, H, W;
		float32                SX, SY, SDX, SDY;
		uint64                 timestamp;
		b2Body*                body;
		bool                   isSpeedSetted;
	public:
		Box2DItem(int t, int x = 0, int y = 0): X(x), Y(y), type(t), status(elsCreating), flags(0), body(NULL), SDX(0.0f), SDY(0.0f), isSpeedSetted(false), timestamp(0) {}
		uint64 getTimestamp() const {return timestamp;}
		void setTimestamp(uint64 t) {timestamp = t;}
		int getType()   const {return type;}
		int getFlags()  const {return flags;}
		int getStatus() const {return status;}
		int getX() const {return X;}
		int getY() const {return Y;}
		virtual int getH() const {return H;}
		virtual int getW() const {return W;}
		void setXY(int x, int y);
		void setSpeed(b2Vec2& v);
		bool getSpeed(b2Vec2& v);
		virtual int getWeight() {return 1;}
		b2Body* getBody() {return body;}
		void setStatus(int s);
		void backup();
		void restore();
};

#endif	// _I_BOX2D_ITEM_H_
