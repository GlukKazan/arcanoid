#ifndef _QUADS_H_
#define _QUADS_H_

#include "IwGX.h"

#define MAX_VERTS 2000

class Quads {
    private:
		CIwSVec2 verts[MAX_VERTS];
		CIwColour cols[MAX_VERTS];
        int     vertc;
    public:
        void update() {vertc = 0;}
        void refresh();
		void setVert(int x, int y, int r, int g, int b, int a);
};

extern Quads quads;

#endif	// _QUADS_H_
