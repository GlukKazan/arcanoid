#ifndef _BALLS_H_
#define _BALLS_H_

#include <map>
#include "Ball.h"
#include "Box2DItem.h"
#include "CommonObject.h"
#include "StringPool.h"

#define CONTACT_SOUND                  "contact"
#define MIN_SOUND_INTERVAL             100

#define TRIPLE_ANGLE                   0.52f
#define EPS                            0.01f
#define MAX_BALLS                      5

#define RADIUS_PROPERTY                "radius"
#define TOP_PROPERTY                   "top"
#define BOTTOM_PROPERTY                "bottom"
#define LEFT_PROPERTY                  "left"
#define RIGHT_PROPERTY                 "right"
#define IMG_PROPERTY                   "image"
#define DEFAULT_PROPERTY               "default"
#define IRON_PROPERTY                  "iron"
#define BOMB_PROPERTY                  "bomb"
#define FIRE_PROPERTY                  "fire"
#define WEIGHT_PROPERTY                "weight"

using namespace std;

class Balls: public CommonObject {
	private:
		struct SBall: public CommonObject {
			SBall(): CommonObject() {clear();}
			SBall(const SBall& p): CommonObject(), radius(p.radius), left(p.left), right(p.right), top(p.top), bottom(p.bottom), img(p.img), isDefault(p.isDefault), type(p.type), weight(p.weight) {}
			SBall& operator=(const SBall& p) {
				if (this != &p) {
					radius = p.radius;
					type = p.type;
					weight = p.weight;
					left = p.left;
					right = p.right;
					top = p.top;
					bottom = p.bottom;
					name = p.name;
					img = p.img;
					isDefault = p.isDefault;
				}
				return *this;
			}
			virtual void clear() {
				radius = type = left = right = top = bottom = 0;
				weight = 1;
				name = NULL;
				img = NULL;
				isDefault = false;
			}
			virtual bool isValid() const {return (name != NULL)&&(radius != 0) && (img != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, RADIUS_PROPERTY) == 0) {
					radius = desktop.toRSize(strToNum(value));
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
				if (strcmp(name, IMG_PROPERTY) == 0) {
					img = pool.string(value);
					return true;
				}
				if (strcmp(name, DEFAULT_PROPERTY) == 0) {
					isDefault = strToBool(value);
					return true;
				}
				if (strcmp(name, IRON_PROPERTY) == 0) {
					weight = 2;
					return true;
				}
				if (strcmp(name, WEIGHT_PROPERTY) == 0) {
					weight = strToNum(value);
					return true;
				}
				if (strcmp(name, BOMB_PROPERTY) == 0) {
					type |= ebtBomb;
					return true;
				}
				if (strcmp(name, FIRE_PROPERTY) == 0) {
					type |= ebtFire;
					return true;
				}
				return false;
			}
			int radius;
			int type;
			int weight;
			int left;
			int right;
			int top;
			int bottom;
			const char* name;
			const char* img;
			bool isDefault;
		};
		map<Box2DItem*, Ball*>* balls;
		SBall currBall;
		map<string, SBall>* ballDefs;
		uint64 lastTimestamp;
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
		void setDefaultImg(Ball* b);
		bool addAngle(b2Vec2& v, float32 delta);
    public:
		Balls() {}
        void init();
        void release();
        void refresh();
        void update();

		void impact(Box2DItem* a, Box2DItem* b);
        void backup();
        void restore();

		Ball* addBall(int x, int y, int s = elsCreating);
		void setBall(Box2DItem* b, const char* name);
		void activateGift(int type);

	typedef map<Box2DItem*, Ball*>::iterator BIter;
	typedef pair<Box2DItem*, Ball*> BPair;
	typedef map<string, SBall>::iterator DIter;
	typedef pair<string, SBall> DPair;
	typedef vector<Ball*>::iterator bIter;
};


#endif	// _BALL_H_
