#include "Handle.h"
#include <string.h>
#include "Quads.h"
#include "TouchPad.h"
#include "ResourceManager.h"
#include "Bricks.h"
#include "Board.h"

void Handle::init(Balls* b) {
	touchId = -1;
	platforms = new map<string, Platform>();
	balls = new set<Box2DItem*>();
	bullets = new map<Box2DItem*, Bullet*>();
	bullet = NULL;
	bc = b;
	isTouchPressed = false;
	lastFire = 0;
    X = desktop.getWidth() / 2;
	Y = desktop.getHeight() - desktop.toRSize(HANDLE_POS);
	isSelected = false;
}

void Handle::release() {
	if (status == elsNornmal) {
		b2Body* body = getBody();
		if (body != NULL) {
			world.dropBody(body);
		}
	}
	for (bIter p = bullets->begin(); p != bullets->end(); ++p) {
		p->second->release();
		delete p->second;
	}
	delete bullets;
	delete platforms;
	delete balls;
}

void Handle::Bullet::release() {
	if (getStatus() == elsNornmal) {
		b2Body* body = getBody();
		if (body != NULL) {
			world.dropBody(body);
		}
	}
}

CommonObject* Handle::createContext(const char* name) {
	currPlatform.clear();
	currPlatform.name = pool.string(name);
	return &currPlatform;
}

bool Handle::closeContext(int scope) {
	if (!currPlatform.isValid()) return false;
//	rm.load(currPlatform.img.c_str());
	platforms->insert(PPair(currPlatform.name, currPlatform));
	clear();
	currPlatform.clear();
	return true;
}

void Handle::impact(Box2DItem* a, Box2DItem* b) {
	checkPlatform();
	if (a == (Box2DItem*)this) {
		if (b == NULL) return;
		if (b->getType() != eitBall) return;
		if (currPlatform.isSticky) {
			uint64 t = s3eTimerGetMs();
			if ((t - b->getTimestamp()) > STICKY_TIMEOUT) {
				b->setStatus(elsDisabling);
				b->setXY(b->getX(), Y - currPlatform.height/2 - (b->getH() >> 1));
				balls->insert(b);
			}
		}
		return;
	}
	for (bIter p = bullets->begin(); p != bullets->end(); ++p) {
		if (p->first == a) {
			p->first->setStatus(elsDeleting);
			break;
		}
	}
}

void Handle::createBullet(int x, int y) {
	if (bullet == NULL) {
		bullet = currPlatform.bullet;
	}
	if (bullet == NULL) return;
	Bullet* b = new Bullet(bullet, x, y);
	bullets->insert(bPair((Box2DItem*)b, b));
}

void Handle::fire() {
	if (currPlatform.isFire) {
		uint64 currTime = s3eTimerGetMs();
		if ((lastFire == 0)||((currTime - lastFire) > FIRE_TIMEOUT)) {
			lastFire = currTime;
			createBullet(X - (currPlatform.width >> 1) + BULLET_WIDTH, 
				Y - (currPlatform.height >> 1) - desktop.toRSize(BULLET_HEIGHT * 2));
			createBullet(X + (currPlatform.width >> 1) - BULLET_WIDTH, 
				Y - (currPlatform.height >> 1) - desktop.toRSize(BULLET_HEIGHT * 2));
		}
	}
}

void Handle::fireAction(EHandleAction e) {
	uint64 t = s3eTimerGetMs();
	switch (e) {
		case eheReleased:
			for (BIter p = balls->begin(); p != balls->end(); ++p) {
				(*p)->setTimestamp(t);
				(*p)->setStatus(elsCreating);
			}
			balls->clear();
		case ehePressed:
			fire();
			break;
		default:
			break;
	}
}

void Handle::setX(int x) {
	checkPlatform();
	int hw = currPlatform.width >> 1;
	if (x - hw < 0) {
		x = hw;
	}
	if (x + hw > desktop.getWidth()) {
		x = desktop.getWidth() - hw;
	}
	int deltaX = x - X;
	if (deltaX != 0) {
		for (BIter p = balls->begin(); p != balls->end(); ++p) {
			int r = (*p)->getH() >> 1;
			int bx = (*p)->getX() + deltaX;
			if (bx - r < 0) bx = r;
			if (bx + r > desktop.getWidth()) bx = desktop.getWidth() - r;
			(*p)->setXY(bx, Y - currPlatform.height/2 - r);
		}
	}
	X = x;
}

void Handle::Bullet::getPosition() {
	if (status != elsNornmal) return;
	if (body != NULL) {
		b2Vec2 pos(body->GetPosition());
		setXY(world.fromWorld(pos.x), world.fromWorld(pos.y));
	}
}

bool Handle::Bullet::isOut() const {
	if (X < 0) return true;
	if (Y < 0) return true;
	if (X > desktop.getWidth()) return true;
	if (Y > desktop.getHeight()) return true;
	return false;
}

Handle::Bullet::Bullet(const char* r, int x, int y): Box2DItem(eitFire, x, y)
	                                               , img(r) {
    W = desktop.toRSize(BULLET_WIDTH);
    H = desktop.toRSize(BULLET_HEIGHT);
}

void Handle::update() {
	checkPlatform();
	for (bIter p = bullets->begin(); p != bullets->end(); ++p) {
		switch (p->first->getStatus()) {
			case elsNornmal:
				p->second->getPosition();
				if (p->second->isOut()) {
					p->second->setStatus(elsDeleting);
				}
				break;
		}
	}
	for (bIter p = bullets->begin(); p != bullets->end(); ++p) {
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
	Touch* t = NULL;
	if (touchId >= 0) {
		t = touchPad.getTouchByID(touchId);
	} else {
		t = touchPad.getTouchPressed();
		if (t == NULL) {
			if (isTouchPressed) {
				fireAction(eheReleased);
				isTouchPressed = false;
			}
		} else {
			if (!isTouchPressed) {
				fireAction(ehePressed);
				isTouchPressed = true;
			}
		}
	}
	if (t != NULL) {
		touchId = t->id;
		if (t->x != X) {
			if (body != NULL) {
				world.dropBody(body);
				body = NULL;
			}
			setX(t->x);
		}
	} else {
		touchId = -1;
	}
	if (body == NULL) {
		body = world.createBody((Box2DItem*)this);
	}
	bool isProceed = true;
	while (isProceed) {
		isProceed = false;
		for (bIter p = bullets->begin(); p != bullets->end(); ++p) {
			if (p->first->getStatus() == elsDeleted) {
				p->second->release();
				delete p->second;
				bullets->erase(p);
				isProceed = true;
				break;
			}
		}
	}
}

void Handle::Bullet::refresh() {
	if (img != NULL) {
		int w = desktop.toRSize(BULLET_WIDTH)  >> 1;
		int h = desktop.toRSize(BULLET_HEIGHT) >> 1;
		rm.paint(img, X - w, Y - h, w * 2, h * 2);
	}
}

void Handle::checkPlatform() {
	if (!isSelected) {
		setDefaultPlatform();
	}
}

void Handle::refresh() {
	checkPlatform();
	for (bIter p = bullets->begin(); p != bullets->end(); ++p) {
		p->second->refresh();
	}
	int ll = currPlatform.left;
	int tt = currPlatform.top;
	int rr = currPlatform.right;
	int bb = currPlatform.bottom;
	rm.paint(currPlatform.img,
		X - (currPlatform.width >> 1), Y - (currPlatform.height >> 1), currPlatform.width, currPlatform.height,
		&ll, &tt, &rr, &bb);
}

int Handle::getH() const {
	if (isSelected) {
		return currPlatform.height;
	} else {
		return Box2DItem::getH();
	}
}

int Handle::getW() const {
	if (isSelected) {
		return currPlatform.width;
	} else {
		return Box2DItem::getW();
	}
}

bool Handle::setDefaultPlatform() {
	for (PIter p = platforms->begin(); p != platforms->end(); ++p) {
		if (p->second.isDefault) {
			isSelected = true;
			currPlatform = p->second;
			return true;
		}
	}
	return false;
}

bool Handle::setPlatform(const char* name) {
	for (PIter p = platforms->begin(); p != platforms->end(); ++p) {
		if (strcmp(p->second.name, name) == 0) {
			isSelected = true;
			currPlatform = p->second;
			return true;
		}
	}
	return setDefaultPlatform();
}

void Handle::addBall() {
	setDefaultPlatform();
	int hh = currPlatform.height >> 1;
	Box2DItem* b = (Box2DItem*)bc->addBall(X, Y - hh, elsDisabled);
	b->setXY(b->getX(), Y - hh - (b->getH() >> 1));
	balls->insert(b);
}

void Handle::activateGift(int type) {
	if (type & egtGun) {
		for (PIter p = platforms->begin(); p != platforms->end(); ++p) {
			if (p->second.isFire) {
				isSelected = true;
				currPlatform = p->second;
				break;
			}
		}
	}
	if (type & egtStick) {
		for (PIter p = platforms->begin(); p != platforms->end(); ++p) {
			if (p->second.isSticky) {
				isSelected = true;
				currPlatform = p->second;
				break;
			}
		}
	}
	if (type & egtBig) {
		const char* nm = currPlatform.next;
		if (nm != NULL) {
			PIter p = platforms->find(nm);
			if (p != platforms->end()) {
				isSelected = true;
				currPlatform = p->second;
			}
		}
	}
	if (type & egtSmall) {
		const char* nm = currPlatform.prev;
		if (nm != NULL) {
			PIter p = platforms->find(nm);
			if (p != platforms->end()) {
				isSelected = true;
				currPlatform = p->second;
			}
		}
	}
}
