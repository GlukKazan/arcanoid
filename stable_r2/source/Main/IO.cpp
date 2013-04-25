#include "s3e.h"

#include "IO.h"

IO io;

void IO::init() {
	touchPad.init();
    KeysAvailable = true; //(s3eKeyboardGetInt(S3E_KEYBOARD_HAS_KEYPAD) || s3eKeyboardGetInt(S3E_KEYBOARD_HAS_ALPHA));
}

void IO::release() {
	touchPad.release();
}

void IO::update() {
	touchPad.update();
	if (KeysAvailable)
		s3eKeyboardUpdate();
}

void IO::refresh() {
	touchPad.clear();
}

bool IO::isKeyDown(s3eKey key) const {
	if (!KeysAvailable) return false;
	return (s3eKeyboardGetState(key) & S3E_KEY_STATE_DOWN) == S3E_KEY_STATE_DOWN;
}

bool IO::isKeyUp(s3eKey key) const {
	if (!KeysAvailable)	return false;
	return (s3eKeyboardGetState(key) & S3E_KEY_STATE_UP) == S3E_KEY_STATE_UP;
}

bool IO::wasKeyPressed(s3eKey key) const {
	if (!KeysAvailable) return false;
	return (s3eKeyboardGetState(key) & S3E_KEY_STATE_PRESSED) == S3E_KEY_STATE_PRESSED;
}

bool IO::wasKeyReleased(s3eKey key) const {
	if (!KeysAvailable) return false;
	return (s3eKeyboardGetState(key) & S3E_KEY_STATE_RELEASED) == S3E_KEY_STATE_RELEASED;
}
