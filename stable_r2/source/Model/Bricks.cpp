#include "Bricks.h"
#include "ResourceManager.h"
#include "Board.h"
#include "World.h"

void Bricks::init() {
	bricks = new map<Box2DItem*, Brick*>();
	gifts = new map<Box2DItem*, Gift*>();
	brickDefs = new map<string, SBrickDef>();
	giftDefs = new map<string, GiftDef>();
	isRandomized = false;
	giftType = 0;
}

void Bricks::clear() {
	for (BIter p = bricks->begin(); p != bricks->end(); ++p) {
		p->second->release();
		delete p->second;
	}
	bricks->clear();
}

void Bricks::release() {
	clear();
	delete bricks;
	for (gIter p = gifts->begin(); p != gifts->end(); ++p) {
		p->second->release();
		delete p->second;
	}
	delete gifts;
	delete brickDefs;
	delete giftDefs;
}

void Bricks::Brick::release() {
	if (getStatus() == elsNornmal) {
		b2Body* body = getBody();
		if (body != NULL) {
			world.dropBody(body);
		}
	}
}

void Bricks::Gift::release() {
	if (getStatus() == elsNornmal) {
		b2Body* body = getBody();
		if (body != NULL) {
			world.dropBody(body);
		}
	}
}

CommonObject* Bricks::createContext(const char* name) {
	if (name[0] != 0) {
		switch (getScope()) {
			case epcDef:
				currDef.clear();
				currDef.name = pool.string(name);
				return &currDef;
			case epcGift:
				currGift.clear();
				currGift.name = pool.string(name);
				return &currGift;
			default:
				return NULL;
		}
	} else {
		setScope(epcBrick);
		currBrick.clear();
		return &currBrick;
	}
}

Bricks::Brick::Brick(const char* r, int x, int y, int g, int b, const char* nm,
					 int w, int h): Box2DItem(eitBrick, x, y)
                                  , img(r)
					              , impact(0) {
	name = nm;
	good = g;
	bad  = b;
    W    = w;
	H    = h;
}

Bricks::Gift::Gift(const char* r, int t, int x, int y): Box2DItem(eitGift, x, y), img(r), type(t) {
	W = desktop.toRSize(GIFT_SIZE);
	H = desktop.toRSize(GIFT_SIZE);
}

void Bricks::addBrick(SBrick& b) {
	DIter p = brickDefs->find(b.type);
	if (p != brickDefs->end()) {
		Brick* brick = new Brick(p->second.img, b.x, b.y, p->second.good, p->second.bad, p->second.name, p->second.width, p->second.height);
		bricks->insert(BPair((Box2DItem*)brick, brick));
	}
}

bool Bricks::closeContext(int scope) {
	if (scope == epcGift) {
		if (!currGift.isValid()) return false;
		giftDefs->insert(GPair(currGift.name, currGift));
		currGift.clear();
		return true;
	}
	if (scope == epcDef) {
		if (!currDef.isValid()) return false;
//		rm.load(currDef.img.c_str());
		brickDefs->insert(DPair(currDef.name, currDef));
		clear();
		currDef.clear();
		return true;
	} else {
		if (!currBrick.isValid()) return false;
		addBrick(currBrick);
		currBrick.clear();
		return true;
	}
}

bool Bricks::isInRect(Box2DItem* a, float32 X, float32 Y) {
	float32 x = (float32)a->getX(); float32 y = (float32)a->getY();
	float32 w = (float32)a->getW(); float32 h = (float32)a->getH();
	if (X < x - w) return false;
	if (Y < y - h) return false;
	if (X > x + w) return false;
	if (Y > y + h) return false;
	return true;
}

bool Bricks::isNear(Box2DItem* a, Box2DItem* b) {
	float32 x = (float32)a->getX(); float32 y = (float32)a->getY();
	float32 w = (float32)a->getW(); float32 h = (float32)a->getH();
	w = w * 0.6f; h = h * 0.6f;
	if (isInRect(b, x - w, y - h)) return true;
	if (isInRect(b, x + w, y - h)) return true;
	if (isInRect(b, x + w, y + h)) return true;
	if (isInRect(b, x - w, y + h)) return true;
	return false;
}

void Bricks::impact(Box2DItem* a, Box2DItem* b) {
	if (b == NULL) return;
	for (gIter p = gifts->begin(); p != gifts->end(); ++p) {
		if (p->first == a) {
			giftType |= p->second->type;
			p->first->setStatus(elsDeleting);
			break;
		}
	}
	for (BIter p = bricks->begin(); p != bricks->end(); ++p) {
		if (p->first == a) {
			int weigth = b->getWeight();
			int flags = b->getFlags();
			p->second->impact += weigth;
			if ((flags & ebtFire) != 0) {
				world.setFired();
			}
			if ((flags & ebtBomb) != 0) {
				for (BIter q = bricks->begin(); q != bricks->end(); ++q) {
					if (q->first == p->first) continue;
					if (isNear(p->first, q->first)) {
						q->second->impact += 1;
					}
				}
			}
			break;
		}
	}
}

const char* Bricks::findNextState(const char* state, int n) {
	for (; n > 0; n--) {
		DIter p = brickDefs->find(state);
		if (p == brickDefs->end()) {
			return NULL;
		}
		state = p->second.next;
		if ((state == NULL)||(state[0] == 0)) {
			return NULL;
		}
	}
	return state;
}

bool Bricks::checkP(int p) {
	if (p == 0) return false;
	if (!isRandomized) {
		isRandomized = true;
		uint64 t = s3eTimerGetMs();
        srand((unsigned int)t);
	}
	int r = rand() % 100;
	if (r <= p) {
		return true;
	}
	return false;
}

bool Bricks::Gift::isOut() const {
	if (X < 0) return true;
	if (Y < 0) return true;
	if (X > desktop.getWidth()) return true;
	if (Y > desktop.getHeight()) return true;
	return false;
}

void Bricks::update() {
	bool isGiftCreated = false;
	if (giftType != 0) {
		board.activateGift(giftType);
		giftType = 0;
	}
	for (BIter p = bricks->begin(); p != bricks->end(); ++p) {
		Brick* brick = p->second;
		switch (p->first->getStatus()) {
			case elsCreating:
				brick->body = world.createBody(p->first);
				brick->setStatus(elsNornmal);
				break;
			case elsNornmal:
				if (p->second->impact > 0) {
					const char* state = findNextState(p->second->name, p->second->impact);
					p->second->impact = 0;
					if (state != NULL) {
						DIter d = brickDefs->find(state);
						if (d != brickDefs->end()) {
							p->second->name = state;
							p->second->img  = d->second.img;
							p->second->good = d->second.good;
							p->second->bad  = d->second.bad;
						} else {
							state = NULL;
						}
					}
					if (state == NULL) {
						p->second->setStatus(elsDeleting);
					}
				}
				break;
		}
	}
	for (BIter p = bricks->begin(); p != bricks->end(); ++p) {
		Brick* brick = p->second;
		switch (p->first->getStatus()) {
			case elsDeleting:
				bool isGift = false;
				bool isGood = false;
				if (checkP(p->second->good)) {
					isGift = true;
					isGood = true;
				} else if (checkP(p->second->bad)) {
					isGift = true;
				}
				if (isGift && !isGiftCreated) {
					GIter g = giftDefs->begin();
					int cn = 0;
					for (; g != giftDefs->end(); ++g) {
						if (g->second.isGood != isGood) continue;
						cn++;
					}
					int r = rand() % cn;
					for (g = giftDefs->begin(); g != giftDefs->end(); ++g, r--) {
						if (g->second.isGood != isGood) continue;
						if (r == 0) break;
					}
					if (g != giftDefs->end()) {
						isGiftCreated = true;
						Gift* r = new Gift(g->second.img, g->second.type, p->first->getX(), p->first->getY());
						gifts->insert(gPair((Box2DItem*)r, r));
					}
				}
				world.dropBody(brick->body);
				p->second->setStatus(elsDeleted);
				break;

		}
	}
	for (gIter p = gifts->begin(); p != gifts->end(); ++p) {
		Gift* gift = p->second;
		if (p->first->getStatus() != elsNornmal) continue;
		p->second->getPosition();
		if (p->second->isOut()) {
			p->second->setStatus(elsDeleting);
		}
	}
	for (gIter p = gifts->begin(); p != gifts->end(); ++p) {
		switch (p->first->getStatus()) {
			case elsCreating:
				p->second->body = world.createBody(p->first);
				p->second->setStatus(elsNornmal);
				break;
			case elsDeleting:
				world.dropBody(p->second->body);
				p->second->setStatus(elsDeleted);
				break;
		}
	}
	bool isProceed = true;
	while (isProceed) {
		isProceed = false;
		for (BIter p = bricks->begin(); p != bricks->end(); ++p) {
			if (p->first->getStatus() == elsDeleted) {
				p->second->release();
				delete p->second;
				bricks->erase(p);
				isProceed = true;
				break;
			}
		}
		for (gIter p = gifts->begin(); p != gifts->end(); ++p) {
			if (p->first->getStatus() == elsDeleted) {
				p->second->release();
				delete p->second;
				gifts->erase(p);
				isProceed = true;
				break;
			}
		}
	}
}

void Bricks::Gift::getPosition() {
	if (status != elsNornmal) return;
	if (body != NULL) {
		b2Vec2 pos(body->GetPosition());
		setXY(world.fromWorld(pos.x), world.fromWorld(pos.y));
	}
}

void Bricks::Gift::refresh() {
	rm.paint(img, X - (W >> 1), Y - (H >> 1), W, H);
}

void Bricks::Brick::refresh() {
	rm.paint(img, X - (W >> 1), Y - (H >> 1), W, H);
}

void Bricks::refresh() {
	for (gIter p = gifts->begin(); p != gifts->end(); ++p) {
		p->second->getPosition();
		p->second->refresh();
	}
	for (BIter p = bricks->begin(); p != bricks->end(); ++p) {
		p->second->refresh();
	}
}
