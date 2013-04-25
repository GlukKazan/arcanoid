#include "IwGX.h"
#include "s3e.h"

#include "Quads.h"

Quads quads;

void Quads::refresh() {
	if (vertc > 0) {
		IwGxSetVertStreamScreenSpace(verts, vertc);
		IwGxSetColStream(cols, vertc);
		IwGxDrawPrims(IW_GX_TRI_LIST, NULL, vertc);
	}
}

void Quads::setVert(int x, int y, int r, int g, int b, int a) {
	if (vertc >= MAX_VERTS) return;
	verts[vertc].x = x;
	verts[vertc].y = y;
    cols[vertc].r  = r; 
	cols[vertc].g  = g; 
	cols[vertc].b  = b; 
	cols[vertc].a  = a;
	vertc++;
}
