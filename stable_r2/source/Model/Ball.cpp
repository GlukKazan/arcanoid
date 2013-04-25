#include "Ball.h"
#include "Desktop.h"
#include "Quads.h"
#include <math.h>
#include "ResourceManager.h"
#include "World.h"

Ball::Ball(int x, int y, int s): Box2DItem(eitBall, x, y)
                               , img(NULL)
							   , weight(1)
							   , isContacted(false)
							   , deltaAngle(0.0f) {
	H = W = desktop.toRSize(BALL_RADIUS * 2);
	status = s;
	b2Vec2 v(INIT_SPEED_X, INIT_SPEED_Y);
	setSpeed(v);
}

Ball::Ball(const Ball& p): Box2DItem(p.type, p.X, p.Y) {
	flags = p.flags;
	L = p.L; T = p.T; B = p.B; R = p.R;
	img = p.img;
	weight = p.weight;
	isContacted = false;
	deltaAngle = 0;
}

void Ball::release() {
	if (status == elsNornmal) {
		b2Body* body = getBody();
		if (body != NULL) {
			world.dropBody(body);
		}
	}
	if (img != NULL) {
		delete img;
	}
}

void Ball::getPosition() {
	if (status != elsNornmal) return;
	if (body != NULL) {
		b2Vec2 pos(body->GetPosition());
		setXY(world.fromWorld(pos.x), world.fromWorld(pos.y));
	}
}

void Ball::refresh() {
	if (img != NULL) {
		int ll = L;
		int tt = T;
		int rr = R;
		int bb = B;
		rm.paint(img->c_str(), 
			X - (W >> 1), Y - (H >> 1), W, H, &ll, &tt, &rr, &bb);
	}
}

bool Ball::isOut() const {
	if (X < 0) return true;
	if (Y < 0) return true;
	if (X > desktop.getWidth()) return true;
	if (Y > desktop.getHeight()) return true;
	return false;
}

void Ball::setImg(int tp, int w, const char* res, int r, int ll, int tt, int rr, int bb) {
	weight = w;
	H = W = r << 1;
	flags = tp;
	L = ll; T = tt; R = rr; B = bb;
	if (img != NULL) {
		delete img;
	}
	img = new string(res);
//	rm.load(res);
}
