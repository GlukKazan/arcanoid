#ifndef _BRICKS_H_
#define _BRICKS_H_

#include "IwGX.h"
#include "s3e.h"

#include "Desktop.h"
#include "World.h"
#include "Box2DItem.h"
#include "CommonObject.h"
#include "StringPool.h"

#define WIDTH_PROPERTY                 "width"
#define HEIGHT_PROPERTY                "height"
#define TOP_PROPERTY                   "top"
#define BOTTOM_PROPERTY                "bottom"
#define LEFT_PROPERTY                  "left"
#define RIGHT_PROPERTY                 "right"
#define IMG_PROPERTY                   "image"
#define NEXT_PROPERTY                  "next"
#define EXPL_PROPERTY                  "explosion"
#define GOOD_PROPERTY                  "good"
#define BAD_PROPERTY                   "bad"

#define TYPE_PROPERTY                  "type"
#define X_PROPERTY                     "x"
#define Y_PROPERTY                     "y"

#define FORK_PROPERTY                  "fork"
#define BOMB_PROPERTY                  "bomb"
#define FIRE_PROPERTY                  "fire"
#define GUN_PROPERTY                   "gun"
#define IRON_PROPERTY                  "iron"
#define LIFE_PROPERTY                  "life"
#define SMALL_PROPERTY                 "small"
#define BIG_PROPERTY                   "big"
#define STICK_PROPERTY                 "stick"

#define GIFT_SIZE                      20

#include <string>
#include <vector>
#include <map>

using namespace std;

enum EGifgtType {
	egtBomb            = 0x0001,
	egtFire            = 0x0002,
	egtIron            = 0x0004,
	egtFork            = 0x0008,
	egtBall            = 0x000F,
	egtGun             = 0x0010,
	egtSmall           = 0x0020,
	egtBig             = 0x0040,
	egtStick           = 0x0080,
	egtPlatform        = 0x01F0,
	egtClear           = 0x0100,
	egtLife            = 0x1000
};

enum EParseContext {
	epcDef             = 1,
	epcBrick           = 2,
	epcGift            = 3
};

class Bricks: public CommonObject {
    private:
		struct SBrick: public CommonObject {
            SBrick(int x = 0, int y = 0): x(x), y(y) {clear();}
			SBrick(const SBrick& p): x(p.x), y(p.y), type(p.type) {}
			SBrick& operator=(const SBrick& p) {
				if (this != &p) {
					x = p.x;
					y = p.y;
					type = p.type;
				}
				return *this;
			}
			virtual void clear() {
				x = y = 0;
				type = NULL;
			}
			virtual bool isValid() const {return (x != 0) && (y != 0) && (type != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, X_PROPERTY) == 0) {
					x = desktop.toRSize(strToNum(value));
					return true;
				}
				if (strcmp(name, Y_PROPERTY) == 0) {
					y = desktop.toRSize(strToNum(value));
					return true;
				}
				if (strcmp(name, TYPE_PROPERTY) == 0) {
					type = pool.string(value);
					return true;
				}
				return false;
			}
            int x, y;
			const char* type;
        };
		struct SBrickDef: public CommonObject {
			SBrickDef(): CommonObject(), next(""), img("") {clear();}
			SBrickDef(const SBrickDef& p): CommonObject(), width(p.width), height(p.height), left(p.left), right(p.right), top(p.top), bottom(p.bottom), next(p.next), img(p.img), isExplosion(p.isExplosion), good(p.good), bad(p.bad), name(p.name) {}
			SBrickDef& operator=(const SBrickDef& p) {
				if (this != &p) {
					width = p.width;
					height = p.height;
					left = p.left;
					right = p.right;
					top = p.top;
					bottom = p.bottom;
					name = p.name;
					next = p.next;
					img = p.img;
					isExplosion = p.isExplosion;
					good = p.good;
					bad = p.bad;
				}
				return *this;
			}
			virtual void clear() {
				width = height = left = right = top = bottom = good = bad = 0;
				name = NULL;
				next = pool.empty();
				img = NULL;
				isExplosion = false;
			}
			virtual bool isValid() const {return (name != NULL) && (width != 0) && (height != 0) && (img != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, WIDTH_PROPERTY) == 0) {
					width = desktop.toRSize(strToNum(value));
					return true;
				}
				if (strcmp(name, HEIGHT_PROPERTY) == 0) {
					height = desktop.toRSize(strToNum(value));
					return true;
				}
				if (strcmp(name, LEFT_PROPERTY) == 0) {
					left = strToNum(value);
					return true;
				}
				if (strcmp(name, RIGHT_PROPERTY) == 0) {
					right = strToNum(value);
					return true;
				}
				if (strcmp(name, TOP_PROPERTY) == 0) {
					top = strToNum(value);
					return true;
				}
				if (strcmp(name, BOTTOM_PROPERTY) == 0) {
					bottom = strToNum(value);
					return true;
				}
				if (strcmp(name, GOOD_PROPERTY) == 0) {
					good = strToNum(value);
					return true;
				}
				if (strcmp(name, BAD_PROPERTY) == 0) {
					bad = strToNum(value);
					return true;
				}
				if (strcmp(name, NEXT_PROPERTY) == 0) {
					next = pool.string(value);
					return true;
				}
				if (strcmp(name, IMG_PROPERTY) == 0) {
					img = pool.string(value);
					return true;
				}
				if (strcmp(name, EXPL_PROPERTY) == 0) {
					isExplosion = strToBool(value);
					return true;
				}
				return false;
			}
			int width;
			int height;
			int left;
			int right;
			int top;
			int bottom;
			int good;
			int bad;
			const char* name;
			const char* next;
			const char* img;
			bool isExplosion;
		};
		struct GiftDef: public CommonObject {
			GiftDef(): CommonObject() {clear();}
			GiftDef(const GiftDef& p): CommonObject(), isGood(p.isGood), name(p.name), img(p.img), type(p.type) {}
			GiftDef& operator=(const GiftDef& p) {
				if (this != &p) {
					isGood = p.isGood;
					name = p.name;
					img = p.img;
					type = p.type;
				}
				return *this;
			}
			virtual void clear() {
				isGood = false;
				name = NULL;
				img = NULL;
				type = 0;
			}
			virtual bool isValid() const {return (name != NULL) && (img != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, IMG_PROPERTY) == 0) {
					img = pool.string(value);
					return true;
				}
				if (strcmp(name, GOOD_PROPERTY) == 0) {
					isGood = strToBool(value);
					return true;
				}
				if (strcmp(name, FORK_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtFork;
					} else {
						type &= ~egtFork;
					}
					return true;
				}
				if (strcmp(name, BOMB_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtBomb;
					} else {
						type &= ~egtBomb;
					}
					return true;
				}
				if (strcmp(name, FIRE_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtFire;
					} else {
						type &= ~egtFire;
					}
					return true;
				}
				if (strcmp(name, GUN_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtGun;
					} else {
						type &= ~egtGun;
					}
					return true;
				}
				if (strcmp(name, IRON_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtIron;
					} else {
						type &= ~egtIron;
					}
					return true;
				}
				if (strcmp(name, LIFE_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtLife;
					} else {
						type &= ~egtLife;
					}
					return true;
				}
				if (strcmp(name, SMALL_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtSmall;
					} else {
						type &= ~egtSmall;
					}
					return true;
				}
				if (strcmp(name, BIG_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtBig;
					} else {
						type &= ~egtBig;
					}
					return true;
				}
				if (strcmp(name, STICK_PROPERTY) == 0) {
					if (strToBool(value)) {
						type |= egtStick;
					} else {
						type &= ~egtStick;
					}
					return true;
				}
				return false;
			}
			bool isGood;
			const char* name;
			const char* img;
			int type;
		};
		class Gift: public Box2DItem {
			private:
				int type;
				const char* img;
			public:
				Gift(const char* r, int t, int x, int y);
				void release();
				void refresh();
				bool isOut() const;
				void getPosition();
			friend class Bricks;
		};
		class Brick: public Box2DItem {
			private:
				int impact;
				int good, bad;
				const char* img;
				const char* name;
			public:
				Brick(const char* r, int x, int y, int g, int b, const char* nm, int w, int h);
				void release();
				void refresh();
			friend class Bricks;
		};
		bool isRandomized;
        map<Box2DItem*, Brick*>* bricks;
		map<Box2DItem*, Gift*>* gifts;
		map<string, SBrickDef>* brickDefs;
		map<string, GiftDef>* giftDefs;
		SBrickDef currDef;
		GiftDef currGift;
		SBrick currBrick;
		int giftType;
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
		const char* findNextState(const char* state, int n);
		bool isNear(Box2DItem* a, Box2DItem* b);
		bool isInRect(Box2DItem* a, float32 X, float32 Y);
		bool checkP(int p);
    public:
		void clear();
		void init();
		void release();
        void update();
        void refresh();

		void impact(Box2DItem* a, Box2DItem* b);
        void addBrick(SBrick& b);

    typedef map<Box2DItem*, Brick*>::iterator BIter;
	typedef pair<Box2DItem*, Brick*> BPair;
	typedef map<string, SBrickDef>::iterator DIter;
	typedef pair<string, SBrickDef> DPair;
	typedef map<string, GiftDef>::iterator GIter;
	typedef pair<string, GiftDef> GPair;
	typedef map<Box2DItem*, Gift*>::iterator gIter;
	typedef pair<Box2DItem*, Gift*> gPair;
};

#endif	// _BRICKS_H_
