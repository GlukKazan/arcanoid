#include "s3e.h"
#include "Balls.h"
#include "Desktop.h"
#include "ResourceManager.h"
#include "Bricks.h"
#include "Board.h"

void Balls::init() {
	balls = new map<Box2DItem*, Ball*>();
	ballDefs = new map<string, SBall>();
	lastTimestamp = 0;
}

void Balls::release() {
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		p->second->release();
		delete p->second;
	}
	delete balls;
	delete ballDefs;
}

CommonObject* Balls::createContext(const char* name) {
	currBall.clear();
	currBall.name = pool.string(name);
	return &currBall;
}

bool Balls::closeContext(int scope) {
	if (!currBall.isValid()) return false;
//	rm.load(currBall.img.c_str());
	ballDefs->insert(DPair(currBall.name, currBall));
	currBall.clear();
	return true;
}

void Balls::backup() {
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		p->first->backup();
	}
}

void Balls::restore() {
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		p->first->restore();
	}
}

void Balls::impact(Box2DItem* a, Box2DItem* b) {
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		if (p->first == a) {
			if (a->getStatus() != elsNornmal) break;
			Ball* ball = p->second;
			ball->isContacted = true;
			uint64 timestamp = s3eTimerGetMs();
			if ((lastTimestamp == 0)||((timestamp - lastTimestamp) > MIN_SOUND_INTERVAL)) {
				lastTimestamp = timestamp;
				rm.fireSound(CONTACT_SOUND);
			}
			break;
		}
	}
}

bool Balls::addAngle(b2Vec2& v, float32 delta) {
	float32 r = sqrt((v.x * v.x) + (v.y * v.y));
	if (r < EPS) return false;
	float32 angle = atan2(v.y, v.x) + delta;
	v.y = r * sin(angle);
	v.x = r * cos(angle);
	return true;
}

void Balls::update() {
	if (balls->empty()) {
		board.addBall();
	}
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		if (p->first->getStatus() != elsNornmal) continue;
		Ball* ball = p->second;
		ball->backup();
		ball->getPosition();
		if (ball->isOut()) {
			ball->setStatus(elsDeleting);
			break;
		}
	}
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		Ball* ball = p->second;
		switch (p->first->getStatus()) {
			case elsCreating:
				ball->body = world.createBody(p->first);
				ball->setStatus(elsNornmal);
				break;
			case elsDeleting:
				world.dropBody(ball->getBody());
				ball->setStatus(elsDeleted);
				break;
			case elsDisabling:
				world.dropBody(ball->getBody());
				ball->setStatus(elsDisabled);
				break;
		}
	}
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		if (p->first->getStatus() != elsNornmal) continue;
		Ball* ball = p->second;
		b2Body* body = ball->getBody();
		if (body == NULL) continue;
		b2Vec2 speed(ball->SDX, ball->SDY);
		if (abs(ball->deltaAngle) > EPS) {
			addAngle(speed, ball->deltaAngle);
			ball->deltaAngle  = 0.0f;
			ball->isContacted = true;
		}
		if (ball->isContacted) {
			ball->isContacted = false;
			world.normalizeSpeed(speed);
		}
		body->SetLinearVelocity(speed);
	}
	bool isProceed = true;
	while (isProceed) {
		isProceed = false;
		for (BIter p = balls->begin(); p != balls->end(); ++p) {
			if (p->first->getStatus() == elsDeleted) {
				p->second->release();
				delete p->second;
				balls->erase(p);
				isProceed = true;
				break;
			}
		}
	}
}

void Balls::refresh() {
	for (BIter p = balls->begin(); p != balls->end(); ++p) {
		Ball* ball = p->second;
		ball->getPosition();
		ball->refresh();
	}
}

void Balls::setDefaultImg(Ball* b) {
	for (DIter p = ballDefs->begin(); p != ballDefs->end(); ++p) {
		if (!p->second.isDefault) continue;
		b->setImg(p->second.type, p->second.weight, p->second.img, p->second.radius, p->second.left, p->second.top, p->second.right, p->second.bottom);
		break;
	}
}

Ball* Balls::addBall(int x, int y, int s) {
	Ball* b = new Ball(x, y, s);
	setDefaultImg(b);
	balls->insert(BPair((Box2DItem*)b, b));
	return b;
}

void Balls::setBall(Box2DItem* b, const char* name) {
	DIter p = ballDefs->find(name);
	BIter q = balls->find(b);
	if ((p != ballDefs->end()) && (q != balls->end())) {
		q->second->setImg(p->second.type, p->second.weight, p->second.img,
			p->second.radius, p->second.left, p->second.top, p->second.right, p->second.bottom);
	}
}

void Balls::activateGift(int type) {
	if (type & egtFork) {
		vector<Ball*> newBalls;
		int cn = 0;
		for (BIter p = balls->begin(); p != balls->end(); ++p) {
			if (p->second->getStatus() != elsNornmal) continue;
			cn++;
		}
		for (BIter p = balls->begin(); p != balls->end(); ++p) {
			if (p->second->getStatus() != elsNornmal) continue;
			if (cn++ >= MAX_BALLS) continue;
			p->second->deltaAngle = -TRIPLE_ANGLE;
			b2Body* b = p->second->getBody();
			if (b == NULL) continue;
			b2Vec2 v(b->GetLinearVelocity());
			Ball* ball = addBall(p->first->getX(), p->first->getY(), p->first->getStatus());
			ball->setStatus(elsCreating);
			addAngle(v, TRIPLE_ANGLE);
			ball->setSpeed(v);
			break;
		}
	}
	if (type & egtIron) {
		for (DIter d = ballDefs->begin(); d != ballDefs->end(); ++d) {
			if (((d->second.type & (ebtFire | ebtBomb)) == 0) && (d->second.weight > 1)) {
				for (BIter p = balls->begin(); p != balls->end(); ++p) {
					p->second->setImg(d->second.type, d->second.weight, d->second.img, d->second.radius,
						d->second.left, d->second.top, d->second.right, d->second.bottom);
				}
				break;
			}
		}
	}
	if (type & egtFire) {
		for (DIter d = ballDefs->begin(); d != ballDefs->end(); ++d) {
			if (d->second.type & ebtFire) {
				for (BIter p = balls->begin(); p != balls->end(); ++p) {
					p->second->setImg(d->second.type, d->second.weight, d->second.img, d->second.radius,
						d->second.left, d->second.top, d->second.right, d->second.bottom);
				}
				break;
			}
		}
	}
	if (type & egtBomb) {
		for (DIter d = ballDefs->begin(); d != ballDefs->end(); ++d) {
			if (d->second.type & ebtBomb) {
				for (BIter p = balls->begin(); p != balls->end(); ++p) {
					p->second->setImg(d->second.type, d->second.weight, d->second.img, d->second.radius,
						d->second.left, d->second.top, d->second.right, d->second.bottom);
				}
				break;
			}
		}
	}
}
