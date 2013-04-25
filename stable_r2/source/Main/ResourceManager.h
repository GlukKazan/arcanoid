#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <map>
#include <string>

#include "s3e.h"
#include "IwGx.h"
#include "IwResManager.h"
#include "IwSound.h"

#include "CommonObject.h"
#include "StringPool.h"

#define SOUND_GROUP    "sounds"
#define MAX_VERTS      2000
#define MAX_RES_NAME   300

#define ID_PROPERTY                    "id"
#define NAME_PROPERTY                  "name"

using namespace std;

class ResourceManager: public CommonObject {
	private:
		struct SImg {
			SImg(): texture(NULL), mat(NULL), width(0), height(0) {}
			SImg(const SImg& p): texture(p.texture), mat(p.mat), width(p.width), height(p.height) {}
			CIwTexture*  texture;
			CIwMaterial* mat;
			int width, height;
		};
		struct Sound: public CommonObject {
			Sound(): CommonObject(), id (0), nm("") {}
			Sound(const Sound& p): CommonObject(), id (p.id), nm(p.nm) {}
			virtual void clear() {
				id = 0;
				nm = NULL;
			}
			virtual bool isValid() const {return (id != 0) && (nm != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, ID_PROPERTY) == 0) {
					id = strToNum(value);
					return true;
				}
				if (strcmp(name, NAME_PROPERTY) == 0) {
					nm = pool.string(value);
					return true;
				}
				return false;
			}
			int id;
			const char* nm;
		};
		bool checkSound() {return true;}
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
		CIwMaterial* load(const char* name, int* w = NULL, int* h = NULL);
		map<string, SImg>* imgs;
		map<int, Sound>* sounds;
		Sound currSound;
        int vertC;
	public:
		void init();
		void release();
		void update();
		void fireSound(const char* name);
		void fireSound(int id);
		void regSound(int id, const char* name);
        void clearPaint();
		void paint(const char* res, int x, int y, int w, int h, int* lm = 0, int* tm = 0, int* rm = 0, int* bm = 0);

	typedef map<string, SImg>::iterator IIter;
	typedef pair<string, SImg> IPair;
	typedef map<int, Sound>::iterator SIter;
	typedef pair<int, Sound> SPair;
};

extern ResourceManager rm;
        
#endif	// _RESOURCEMANAGER_H_
