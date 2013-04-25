#ifndef _IO_H_
#define _IO_H_

#include "TouchPad.h"

class IO {
    private:
        bool KeysAvailable;
    public:
        void init();
        void release();
        void update();
        void refresh();
	    bool isKeyDown(s3eKey key) const;
        bool isKeyUp(s3eKey key) const;
	    bool wasKeyPressed(s3eKey key) const;
	    bool wasKeyReleased(s3eKey key) const;
};

extern IO io;

#endif	// _IO_H_
