#include "Desktop.h"

Desktop desktop;

void Desktop::init() {
    IwGxInit();
    IwGxLightingOff();
    width = IwGxGetScreenWidth();
    height = IwGxGetScreenHeight();
	IwGxSetColClear(0, 0, 0, 0);
    vSize = 0;
	duration = 1000 / 60;
	devices = new vector<Device>();
	defPath = NULL;
}

void Desktop::release() {
	clear();
	delete devices;
    IwGxTerminate();
}

void Desktop::clear() {
	if (defPath != NULL) {
		delete defPath;
		defPath = NULL;
	}
}

void Desktop::update() {
    IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
}

void Desktop::refresh() {
    IwGxFlush();
    IwGxSwapBuffers();
    s3eDeviceYield(duration);   
}

int Desktop::toRSize(int x) const {
    if (vSize == 0) return x;
    return (x * width) / vSize;
}

CommonObject* Desktop::createContext(const char* name) {
	currDevice.clear();
	return &currDevice;
}

bool Desktop::closeContext(int scope) {
	if (!currDevice.isValid()) return false;
	devices->push_back(currDevice);
	return true;
}

const char* Desktop::getDevPath() {
	if (defPath == NULL) {
		for (DIter p = devices->begin(); p != devices->end(); ++p) {
			if (p->isDefault) {
				defPath = new string(p->path);
				break;
			}
		}
	}
	if (defPath == NULL) {
		return "";
	}
	return defPath->c_str();
}
