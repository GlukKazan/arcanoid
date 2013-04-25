#include "Locale.h"
#include "s3e.h"

Locale loc;

void Locale::init() {
	langs = new map<int, Lang>();
	defLangPath = NULL;
}

void Locale::release() {
	clear();
	delete langs;
}

void Locale::clear() {
	if (defLangPath != NULL) {
		delete defLangPath;
		defLangPath = NULL;
	}
}


CommonObject* Locale::createContext(const char* name) {
	currLang.clear();
	return &currLang;
}

bool Locale::closeContext(int scope) {
	if (!currLang.isValid()) return false;
	langs->insert(LPair(currLang.id, currLang));
	clear();
	return true;
}

const char* Locale::getLangPath() {
	if (defLangPath == NULL) {
		int32 lang = s3eDeviceGetInt(S3E_DEVICE_LANGUAGE);
		for (LIter p = langs->begin(); p != langs->end(); ++p) {
			if (p->second.isDefault || (p->second.id == lang)) {
				clear();
				defLangPath = new string(p->second.path);
			}
			if (p->second.id == lang) break;
		}
	}
	return defLangPath->c_str();
}
