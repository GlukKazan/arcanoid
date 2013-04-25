#ifndef _BOARD_H_
#define _BOARD_H_

#include <yaml.h>
#include <vector>
#include <map>
#include <stack>
#include <String>

#include "DataLoader.h"
#include "CommonObject.h"
#include "StringPool.h"

#include "Bricks.h"
#include "Balls.h"
#include "Handle.h"

using namespace std;

#define BOARD_CONTEXT                  "board"
#define BACKGROUNDS_CONTEXT            "backgrounds"
#define LEVELS_CONTEXT                 "levels"
#define SOUNDS_CONTEXT                 "sounds"
#define LANGS_CONTEXT                  "langs"
#define DEVICES_CONTEXT                "devices"
#define PLATFORMS_CONTEXT              "platforms"
#define BRICKS_CONTEXT                 "bricks"
#define BALLS_CONTEXT                  "balls"
#define GIFTS_CONTEXT                  "gifts"

#define WIDTH_PROPERTY                 "width"
#define BACKGROUND_PROPERTY            "background"
#define START_PROPERTY                 "start"
#define FILE_PROPERTY                  "file"
#define NEXT_PROPERTY                  "next"

enum EContextType {
	ectUnknown      = 0,
	ectBoard        = 1,
	ectBackground   = 2,
	ectLevels       = 3,
	ectLevel        = 4

};

class Board: public CommonObject {
    private:
		struct Context {
			Context(CommonObject* prev, int type): prevContext(prev), contextType(type) {}
			Context(const Context& p): prevContext(p.prevContext), contextType(p.contextType) {}
			CommonObject* prevContext;
			int contextType;
		};
		struct Level: public CommonObject {
			Level(): CommonObject(), name(""), file(""), next("") {}
			Level(const Level& p): CommonObject(), name(p.name), file(p.file), next(p.next) {}
			virtual void clear() {
				name = NULL;
				file = NULL;
				next = pool.empty();
			}
			virtual bool isValid() const {return (file != NULL) && (name != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, FILE_PROPERTY) == 0) {
					file = pool.string(value);
					return true;
				}
				if (strcmp(name, NEXT_PROPERTY) == 0) {
					next = pool.string(value);
					return true;
				}
				return false;
			}
			const char* name;
			const char* file;
			const char* next;
		};
		string* img;
		string* background;
		string* startLevel;
		Level currLevel;
		map<string, Level>* levels;
        Bricks bricks;
        Balls  balls;
		Handle handle;
		stack<Context>* contexts;
		map<string, string>* backgrounds;
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
    public:
		Board() {}
        void init();
        void release();
        void refresh();
        void update();
		void activateGift(int type);

		void impact(Box2DItem* a, Box2DItem* b);
		void backup();
		void restore();

		void updateBricks();
		void addBall() {handle.addBall();}

		virtual void        clear();
		virtual bool        setValue(const char* name, const char* value);

    typedef vector<string>::iterator SIter;
	typedef map<string, string>::iterator BIter;
	typedef pair<string, string> BPair;
	typedef map<string, Level>::iterator LIter;
	typedef pair<string, Level> LPair;
};

extern Board board;

#endif	// _BOARD_H_
