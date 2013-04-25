#include "Box2DItem.h"
#include "World.h"

void Box2DItem::setStatus(int s) {
	switch(s) {
		case elsDeleted:
		case elsDisabled:
			body = NULL;
	}
	status = s;
}

void Box2DItem::setXY(int x, int y) {
	X = x;
	Y = y;
}

void Box2DItem::setSpeed(b2Vec2& v) {
	SDX = v.x;
	SDY = v.y;
	isSpeedSetted = true;
}

bool Box2DItem::getSpeed(b2Vec2& v) {
	if (!isSpeedSetted) return false;
	isSpeedSetted = false;
	v.x = SDX;
	v.y = SDY;
	return true;
}

void Box2DItem::backup() {
	if (body != NULL) {
		b2Vec2 pos(body->GetPosition());
		SX = pos.x; SY = pos.y;
		b2Vec2 speed(body->GetLinearVelocity());
		SDX = speed.x; SDY = speed.y;
	}
}

void Box2DItem::restore() {
	if (body != NULL) {
		X = world.fromWorld(SX);
		Y = world.fromWorld(SY);
		world.dropBody(body);
		body = world.createBody(this);
		body->SetLinearVelocity(b2Vec2(SDX, SDY));
	}
}
