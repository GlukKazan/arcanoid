#include "Board.h"
#include "Desktop.h"
#include "ResourceManager.h"
#include "Locale.h"

Board board;

void Board::init() {
	contexts = new stack<Context>();
	backgrounds = new map<string, string>();
	levels = new map<string, Level>();
	img = NULL;
	background = NULL;
	startLevel = NULL;
	bricks.init();
    balls.init();
	handle.init(&balls);
	DataLoader loader;
	loader.load("main.json", this);
	if (startLevel != NULL) {
		LIter p = levels->find(*startLevel);
		if (p != levels->end()) {
			loader.load(p->second.file, this);
		}
	}
	handle.addBall();
}

void Board::release() {
	if (img != NULL) {
		delete img;
	}
	if (background != NULL) {
		delete background;
	}
	if (startLevel != NULL) {
		delete startLevel;
	}
	handle.release();
	bricks.release();
	balls.release();
	delete contexts;
	delete backgrounds;
	delete levels;
}

void Board::update() {
	bricks.update();
	balls.update();
	handle.update();
	balls.update();
}

void Board::updateBricks() {
	bricks.update();
}

void Board::impact(Box2DItem* a, Box2DItem* b) {
	bricks.impact(a, b);
	handle.impact(a, b);
	balls.impact(a, b);
}

void Board::backup() {
	balls.backup();
//	handle.backup();
}

void Board::restore() {
	balls.restore();
//	handle.restore();
}

void Board::refresh() {
    rm.clearPaint();
	if (background != NULL) {
		if (img == NULL) {
			BIter p = backgrounds->find(*background);
			if (p != backgrounds->end()) {
				img = new string(p->second);
			} else {
				delete background;
				background = NULL;
			}
		}
		if (img != NULL) {
			rm.paint(img->c_str(), 0, 0, desktop.getWidth(), desktop.getHeight());
		}
	}
    bricks.refresh();
    balls.refresh();
    handle.refresh();
}

void Board::clear() {
    bricks.clear();
}

CommonObject* Board::createContext(const char* name) {
	if (strcmp(name, BOARD_CONTEXT) == 0) {
		setScope(ectBoard);
		return this;
	}
	if (strcmp(name, BACKGROUNDS_CONTEXT) == 0) {
		setScope(ectBackground);
		return this;
	}
	if (strcmp(name, LEVELS_CONTEXT) == 0) {
		setScope(ectLevels);
		return this;
	}
	if (getScope() == ectLevels) {
		setScope(ectLevel);
		currLevel.clear();
		currLevel.name = pool.string(name);
		currLevel.setScope(ectUnknown);
		return &currLevel;
	}
	if (strcmp(name, SOUNDS_CONTEXT) == 0) {
		return &rm;
	}
	if (strcmp(name, LANGS_CONTEXT) == 0) {
		return &loc;
	}
	if (strcmp(name, DEVICES_CONTEXT) == 0) {
		return &desktop;
	}
	if (strcmp(name, PLATFORMS_CONTEXT) == 0) {
		return &handle;
	}
	if (strcmp(name, BRICKS_CONTEXT) == 0) {
		bricks.setScope(epcDef);
		return &bricks;
	}
	if (strcmp(name, GIFTS_CONTEXT) == 0) {
		bricks.setScope(epcGift);
		return &bricks;
	}
	if (strcmp(name, BALLS_CONTEXT) == 0) {
		return &balls;
	}
	return this;
}

bool Board::closeContext(int scope) {
	switch (scope) {
		case ectLevel:
			if (!currLevel.isValid()) return false;
			levels->insert(LPair(currLevel.name, currLevel));
			return true;
		default:
			break;
	}
	return true;
}

bool Board::setValue(const char* name, const char* value) {
	int t = getScope();
	switch (t) {
		case ectBoard:
			if (strcmp(name, WIDTH_PROPERTY) == 0) {
				desktop.setVSize(strToNum(value));
				return true;
			}
			if (strcmp(name, BACKGROUND_PROPERTY) == 0) {
				if (background != NULL) {
					delete background;
				}
				background = new string(value);
				return true;
			}
			if (strcmp(name, START_PROPERTY) == 0) {
				if (startLevel != NULL) {
					delete startLevel;
				}
				startLevel = new string(value);
				return true;
			}
			return false;
		case ectBackground:
			backgrounds->insert(BPair(string(name), string(value)));
			return true;
		default:
			return false;
	}
}

void Board::activateGift(int type) {
	if ((type & egtBall) != 0) {
		balls.activateGift(type);
	}
	if ((type & egtPlatform) != 0) {
		handle.activateGift(type);
	}
	// TODO: egtLife

}
