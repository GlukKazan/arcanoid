#ifndef _HANDLE_H_
#define _HANDLE_H_

#include <string>
#include <map>
#include <set>

#include "IwGX.h"
#include "s3e.h"

#include "Desktop.h"
#include "World.h"
#include "Box2DItem.h"
#include "CommonObject.h"
#include "StringPool.h"
#include "Balls.h"

#define IMPACT_TIMEOUT                 500

#define HANDLE_WIDTH                   40
#define HANDLE_HEIGHT                  10
#define HANDLE_POS                     70
#define BULLET_WIDTH                   4
#define BULLET_HEIGHT                  22
#define FIRE_TIMEOUT                   500
#define STICKY_TIMEOUT                 500

#define WIDTH_PROPERTY                 "width"
#define HEIGHT_PROPERTY                "height"
#define TOP_PROPERTY                   "top"
#define BOTTOM_PROPERTY                "bottom"
#define LEFT_PROPERTY                  "left"
#define RIGHT_PROPERTY                 "right"
#define TIMELIFE_PROPERTY              "timelife"
#define NEXT_PROPERTY                  "next"
#define PREV_PROPERTY                  "prev"
#define IMG_PROPERTY                   "image"
#define FIRE_PROPERTY                  "fire"
#define STICKY_PROPERTY                "sticky"
#define DEFAULT_PROPERTY               "default"
#define BULLET_PROPERTY                "bullet"

enum EHandleAction {
	ehePressed           = 1,
	eheReleased          = 2
};

class Handle: public CommonObject
	        , public Box2DItem {
	private:
		struct Platform: public CommonObject {
			Platform(): CommonObject() {clear();}
			Platform(const Platform& p): CommonObject(), width(p.width), height(p.height), left(p.left), right(p.right), top(p.top), bottom(p.bottom), next(p.next), img(p.img), isFire(p.isFire), isSticky(p.isSticky), timeLife(p.timeLife), isDefault(p.isDefault), bullet(p.bullet), prev(p.prev) {}
			Platform& operator=(const Platform& p) {
				if (this != &p) {
					width = p.width;
					height = p.height;
					left = p.left;
					right = p.right;
					top = p.top;
					bottom = p.bottom;
					timeLife = p.timeLife;
					name = p.name;
					next = p.next;
					prev = p.prev;
					img = p.img;
					bullet = p.bullet;
					isFire = p.isFire;
					isSticky = p.isSticky;
					isDefault = p.isDefault;
				}
				return *this;
			}
			virtual void clear() {
				width = height = left = right = top = bottom = timeLife = 0;
				name = NULL;
				next = pool.empty();
				prev = pool.empty();
				img = NULL;
				bullet = NULL;
				isFire = false;
				isSticky = false;
				isDefault = false;
			}
			virtual bool isValid() const {
				if (isFire && (bullet == NULL)) return false;
				return (name != NULL)&&(width != 0) && (height != 0) && (img != NULL);
			}
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
				if (strcmp(name, TIMELIFE_PROPERTY) == 0) {
					timeLife = strToNum(value);
					return true;
				}
				if (strcmp(name, NEXT_PROPERTY) == 0) {
					next = pool.string(value);
					return true;
				}
				if (strcmp(name, PREV_PROPERTY) == 0) {
					prev = pool.string(value);
					return true;
				}
				if (strcmp(name, IMG_PROPERTY) == 0) {
					img = pool.string(value);
   					return true;
				}
				if (strcmp(name, BULLET_PROPERTY) == 0) {
					bullet = pool.string(value);
   					return true;
				}
				if (strcmp(name, FIRE_PROPERTY) == 0) {
					isFire = strToBool(value);
					return true;
				}
				if (strcmp(name, STICKY_PROPERTY) == 0) {
					isSticky = strToBool(value);
					return true;
				}
				if (strcmp(name, DEFAULT_PROPERTY) == 0) {
					isDefault = strToBool(value);
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
			int timeLife;
			const char* name;
			const char* next;
			const char* prev;
			const char* img;
			const char* bullet;
			bool isFire;
			bool isSticky;
			bool isDefault;
		};
		struct Bullet: public Box2DItem {
			private:
				const char* img;
			public:
				Bullet(const char* r, int x, int y);
				void release();
				void refresh();
				void getPosition();
				bool isOut() const;
			friend class Handle;
		};
		bool     isSelected;
		int      touchId;
		bool     isTouchPressed;
		Platform currPlatform;
		map<string, Platform>* platforms;
		set<Box2DItem*>* balls;
		map<Box2DItem*, Bullet*>* bullets;
		const char* bullet;
		uint64 lastFire;
		Balls* bc;
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
		void checkPlatform();
		void fireAction(EHandleAction e);
		void fire();
		void createBullet(int x, int y);
		void setX(int x);
	public:
		Handle(): Box2DItem(eitPlatform) {}
		void addBall();
		void impact(Box2DItem* a, Box2DItem* b);

        void init(Balls* b);
        void release();
        void refresh();
        void update();

		bool setDefaultPlatform();
		bool setPlatform(const char* name);
		void activateGift(int type); 

		virtual int getH() const;
		virtual int getW() const;

	typedef map<string, Platform>::iterator PIter;
	typedef pair<string, Platform> PPair;
	typedef set<Box2DItem*>::iterator BIter;
	typedef map<Box2DItem*, Bullet*>::iterator bIter;
	typedef pair<Box2DItem*, Bullet*> bPair;
};

#endif	// _HANDLE_H_
