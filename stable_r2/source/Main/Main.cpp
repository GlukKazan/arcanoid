#include "Main.h"

#include "s3e.h"
#include "IwGx.h"
#include "IwSound.h"

#include "Locale.h"
#include "Desktop.h"
#include "Quads.h"
#include "IO.h"
#include "World.h"
#include "ResourceManager.h"
#include "Board.h"

void init() {
    s3eSurfaceSetInt(S3E_SURFACE_DEVICE_ORIENTATION_LOCK, S3E_SURFACE_PORTRAIT_FIXED); 
    desktop.init();
	loc.init();
    io.init();
	rm.init();
    world.init();
    board.init();
}

void release() {
	board.release();
  	world.release();
	rm.release();
    io.release();
	loc.release();
    desktop.release();
}

int main() {
    init(); {
        while (!s3eDeviceCheckQuitRequest()) {
            io.update();
			if (io.isKeyDown(s3eKeyAbsBSK) || io.isKeyDown(s3eKeyBack) || io.isKeyDown(s3eKeyLSK)) {
				break;
			}
			world.update();
 			board.update();

            desktop.update();
			quads.update();
			rm.update();
            board.refresh();

			quads.refresh();
            io.refresh();
            desktop.refresh();
        }
    }
    release();
    return 0;
}
