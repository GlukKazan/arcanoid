#ifndef _DESKTOP_H_
#define _DESKTOP_H_

#include <string>
#include <vector>
#include "IwGX.h"
#include "s3e.h"

#include "CommonObject.h"
#include "StringPool.h"

#define PATH_PROPERTY                  "path"
#define DEFAULT_PROPERTY               "default"
#define WIDTH_PROPERTY                 "width"
#define HEIGHT_PROPERTY                "height"

enum EMessageType {
	emtNothing                                                      = 0x00,

	emtHide                                                         = 0x01,
	emtShadow                                                       = 0x02,
	emtShow                                                         = 0x03,
	emtSwitch                                                       = 0x04,
	emtInit                                                         = 0x05,
	emtFix                                                          = 0x08,
	emtClose                                                        = 0x0C,
	emtOpen                                                         = 0x0D,

	emtStartAnimation                                               = 0x06,
	emtStopAnimation                                                = 0x07,
	emtActivate                                                     = 0x09,

	emtPause                                                        = 0x0A,
	emtResume                                                       = 0x0B,

	emtSystemMessage                                                = 0x0F,

	emtTouchEvent                                                   = 0x10,
	emtTouchIdMask                                                  = 0x03,
    emtTouchMask                                                    = 0x78,
    emtMultiTouch                                                   = 0x14,
    emtTouchOut                                                     = 0x18,
	emtTouchDown                                                    = 0x30,
	emtTouchUp                                                      = 0x50,
	emtTouchOutUp                                                   = 0x58,
	emtTouchMove                                                    = 0x70,
	emtSingleTouchDown                                              = 0x30,
	emtSingleTouchUp                                                = 0x50,
	emtSingleTouchMove                                              = 0x70,
	emtMultiTouchDown                                               = 0x34,
	emtMultiTouchUp                                                 = 0x54,
	emtMultiTouchMove                                               = 0x74,

	emtKeyEvent                                                     = 0x80,
	emtKeyAction                                                    = 0x82,
	emtKeyDown                                                      = 0x81,
	emtKeyPressed                                                   = 0x83,
	emtKeyReleased                                                  = 0x82
};

using namespace std;

class Desktop: public CommonObject {
    private:
		struct Device: public CommonObject {
			Device(): CommonObject(), path(""), isDefault(false), width(0), height(0) {}
			Device(const Device& p): CommonObject(), path(p.path), isDefault(p.isDefault), width(p.width), height(p.height) {}
			virtual void clear() {
				width = 0;
				height = 0;
				path = NULL;
				isDefault = false;
			}
			virtual bool isValid() const {return (path != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, PATH_PROPERTY) == 0) {
					path = pool.string(value);
					return true;
				}
				if (strcmp(name, DEFAULT_PROPERTY) == 0) {
					isDefault = strToBool(value);
					return true;
				}
				if (strcmp(name, WIDTH_PROPERTY) == 0) {
					width = strToNum(value);
					return true;
				}
				if (strcmp(name, HEIGHT_PROPERTY) == 0) {
					height = strToNum(value);
					return true;
				}
				return false;
			}
			int width;
			int height;
			const char* path;
			bool isDefault;
		};
        int width;
        int height;
        int vSize;
		int duration;
		Device currDevice;
		vector<Device>* devices;
		string* defPath;
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
		virtual void clear();
    public:
        void init();
        void release();
        void update();
        void refresh();
        int  getWidth() const {return width;}
        int  getHeight() const {return height;}
        void setVSize(int v) {vSize = v;}
        int  toRSize(int x) const;
		const char* getDevPath();

	typedef vector<Device>::iterator DIter;
};

extern Desktop desktop;

#endif	// _DESKTOP_H_
