#ifndef _LOCALE_H_
#define _LOCALE_H_

#include <string>
#include <map>

#include "CommonObject.h"
#include "StringPool.h"

#define ID_PROPERTY                    "id"
#define PATH_PROPERTY                  "path"
#define DEFAULT_PROPERTY               "default"

using namespace std;

class Locale: public CommonObject {
	private:
		struct Lang: public CommonObject {
			Lang(): CommonObject(), id(0), path(""), isDefault(false) {}
			Lang(const Lang& p): CommonObject(), id (p.id), path(p.path), isDefault(p.isDefault) {}
			virtual void clear() {
				id = 0;
				path = NULL;
				isDefault = false;
			}
			virtual bool isValid() const {return (id != 0) && (path != NULL);}
		    virtual bool setValue(const char* name, const char* value) {
				if (strcmp(name, ID_PROPERTY) == 0) {
					id = strToNum(value);
					return true;
				}
				if (strcmp(name, PATH_PROPERTY) == 0) {
					path = pool.string(value);
					return true;
				}
				if (strcmp(name, DEFAULT_PROPERTY) == 0) {
					isDefault = strToBool(value);
					return true;
				}
				return false;
			}
			int id;
			const char* path;
			bool isDefault;
		};
		map<int, Lang>* langs;
		Lang currLang;
		string* defLangPath;
		virtual CommonObject* createContext(const char* name);
		virtual bool closeContext(int scope);
		virtual void clear();
	public:
		Locale(): CommonObject(), defLangPath(NULL) {}
		void init();
		void release();
		const char* getLangPath();

	typedef map<int, Lang>::iterator LIter;
	typedef pair<int, Lang> LPair;
};

extern Locale loc;

#endif	// _LOCALE_H_
