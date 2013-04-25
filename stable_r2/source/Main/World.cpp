#include "s3e.h"
#include "ResourceManager.h"
#include "World.h"
#include "Board.h"

World world;

void World::init() {
	bodies = new set<b2Body*>();
	int width  = desktop.getWidth();
	int height = desktop.getHeight();
	b2Vec2 gravity(0.0f, GRAVITY);
	wp = new b2World(gravity);
	wp->SetContactListener(this);
	createBox(width/2, -HALF_MARGIN, width/2, HALF_MARGIN);
	createBox(-HALF_MARGIN, height/2, HALF_MARGIN, height/2);
//	createBox(width/2, height + HALF_MARGIN, width/2, HALF_MARGIN);
	createBox(width + HALF_MARGIN, height/2, HALF_MARGIN, height/2);
}

void World::release() {
	delete wp;
	delete bodies;
}

b2Body* World::createBox(int x, int y, int hw, int hh) {
	b2BodyDef def;
	def.type = b2_staticBody;
	def.position.Set(toWorld(x), toWorld(y));
	b2Body* r = wp->CreateBody(&def);
	b2PolygonShape box;
	box.SetAsBox(toWorld(hw), toWorld(hh));
	b2FixtureDef fd;
	fd.shape = &box;
	fd.filter.categoryBits = 0x0001;
	fd.filter.maskBits = 0x0002;
	fd.density = 0;
	fd.friction = FRICTION;
	fd.restitution = RESTITUTION;
	r->CreateFixture(&fd);
	return r;
}

b2Body* World::createPlatform(int x, int y, int hw, int hh) {
	b2BodyDef def;
	def.type = b2_staticBody;
	def.position.Set(toWorld(x), toWorld(y));
	b2Body* handle = wp->CreateBody(&def);
	b2PolygonShape box;
	box.SetAsBox(toWorld(hw), toWorld(hh));
	b2FixtureDef fd;
	fd.shape = &box;
	fd.filter.categoryBits = 0x0008;
	fd.filter.maskBits = 0x0006;
	fd.density = DYN_DENSITY;
	fd.friction = FRICTION;
	fd.restitution = RESTITUTION;
	handle->CreateFixture(&fd);
	return handle;
}

b2Body* World::createBall(int x, int y, int r) {
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.linearDamping = 0.0f;
	def.angularDamping = 0.0f;
	def.position.Set(toWorld(x), toWorld(y));
	b2Body* ball = wp->CreateBody(&def);
	b2CircleShape shape;
	shape.m_p.SetZero();
	shape.m_radius = toWorld(r);
	b2FixtureDef fd;
	fd.filter.categoryBits = 0x0002;
	fd.filter.maskBits = 0x0009;
	fd.shape = &shape;
	fd.density = DYN_DENSITY;
	fd.friction = FRICTION;
	fd.restitution = RESTITUTION;
	ball->CreateFixture(&fd);
	ball->SetBullet(true);
	return ball;
}

b2Body* World::createBullet(int x, int y, int hw, int hh) {
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.linearDamping = 0.0f;
	def.angularDamping = 0.0f;
	def.position.Set(toWorld(x), toWorld(y));
	b2Body* bullet = wp->CreateBody(&def);
	b2PolygonShape box;
	box.SetAsBox(toWorld(hw), toWorld(hh));
	b2FixtureDef fd;
	fd.shape = &box;
	fd.filter.categoryBits = 0x0002;
	fd.filter.maskBits = 0x0001;
	fd.density = DYN_DENSITY;
	fd.friction = FRICTION;
	fd.restitution = 0;
	bullet->CreateFixture(&fd);
	bullet->SetBullet(true);
	bullet->SetLinearVelocity(b2Vec2(0, - BULLET_SPEED));
	return bullet;
}

b2Body* World::createGift(int x, int y, int r) {
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.linearDamping = 0.0f;
	def.angularDamping = 0.0f;
	def.position.Set(toWorld(x), toWorld(y));
	b2Body* gift = wp->CreateBody(&def);
	b2PolygonShape box;
	box.SetAsBox(toWorld(r), toWorld(r));
	b2FixtureDef fd;
	fd.shape = &box;
	fd.filter.categoryBits = 0x0004;
	fd.filter.maskBits = 0x0008;
	fd.density = DYN_DENSITY;
	fd.friction = FRICTION;
	fd.restitution = 0;
	gift->CreateFixture(&fd);
	gift->SetBullet(true);
	gift->SetLinearVelocity(b2Vec2(0, GIFT_SPEED));
	return gift;
}

void World::dropBody(b2Body* b) {
	BIter p = bodies->find(b);
	if (p != bodies->end()) {
		wp->DestroyBody(b);
		bodies->erase(p);
	}
}

b2Body* World::createBody(Box2DItem* it) {
	b2Vec2 v;
	b2Body* r = NULL;
	switch (it->getType()) {
		case eitPlatform:
			r = createPlatform(it->getX(), it->getY(), it->getW() >> 1, it->getH() >> 1);
			break;
		case eitBrick:
			r = createBox(it->getX(), it->getY(), it->getW() >> 1, it->getH() >> 1);
			break;
		case eitBall:
			r = createBall(it->getX(), it->getY(), it->getW() >> 1);
			if (it->getSpeed(v)) {
				r->SetLinearVelocity(v);
			}
			break;
		case eitFire:
			r = createBullet(it->getX(), it->getY(), it->getW() >> 1, it->getH() >> 1);
			r->SetLinearVelocity(b2Vec2(0, -BULLET_SPEED));
			break;
		case eitGift:
			r = createGift(it->getX(), it->getY(), it->getW() >> 1);
			r->SetLinearVelocity(b2Vec2(0, GIFT_SPEED));
			break;
	}
	if (r != NULL) {
		r->SetUserData(it);
		bodies->insert(r);
	}
	return r;
}

void World::normalize(b2Vec2& v) {
	float32 q = sqrt((v.x * v.x) + (v.y * v.y));
	v.x = (v.x * BALL_SPEED) / q;
	v.y = (v.y * BALL_SPEED) / q;
}

float32 World::sign(float32 x) {
	if (x < 0) {
		return -1.0f;
	} else {
		return 1.0f;
	}
}

void World::normalizeSpeed(b2Vec2& v) {
		if (abs(v.x) <= SPEED_EPS) {
			v.x = v.x * 2.0f;
			if (v.x < 0.5) {
			    int r = rand() % 2;
				v.x = 1.0f;
				if (r == 0) {
					v.x = -v.x;
				}
			}
			float32 d = BALL_SPEED - (v.x * v.x); 
			if (d > 0) {
				v.y = d * sign(v.y);
			}
		}
		if (abs(v.y) <= SPEED_EPS) {
			v.y = v.y * 2.0f;
			if (v.y < 0.5) {
				int r = rand() % 2;
				v.y = 1.0f;
				if (r == 0) {
					v.y = -v.y;
				}
			}
			float32 d = BALL_SPEED - (v.y * v.y); 
			if (d > 0) {
				v.x = d * sign(v.x);
			}
		}
		float32 d = BALL_SPEED - (v.x * v.x); 
		if (d > 0) {
			v.y = d * sign(v.y);
		}
		normalize(v);
}

void World::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	b2Body* a = contact->GetFixtureA()->GetBody();
	b2Body* b = contact->GetFixtureB()->GetBody();
	board.impact((Box2DItem*)a->GetUserData(), (Box2DItem*)b->GetUserData());
	board.impact((Box2DItem*)b->GetUserData(), (Box2DItem*)a->GetUserData());
}

float32 World::getTimeStep() {
	float32 r = 30.0f;
	return (float32)r / 1000.0f;
}

void World::update() {
	isFired = false;
	board.backup();
	wp->Step(getTimeStep(), V_ITERATIONS, P_ITERATIONS);
	while (isFired) {
		isFired = false;
		board.updateBricks();
		board.restore();
		wp->Step(getTimeStep(), V_ITERATIONS, P_ITERATIONS);
	}
}
