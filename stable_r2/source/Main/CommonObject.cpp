#include "CommonObject.h"
#include <string.h>

SContext contextStack[MAX_CONTEXT_DEPTH];
int      contextStackP = 0;

int CommonObject::strToNum(const char* s) {
    int r = 0;
    int x = 10;
    for (size_t i = 0; i < strlen(s); i++) {
        switch (s[i]) {
            case 'x':
            case 'X':
                x = 16;
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                x = 16;
                r *= x;
                r += s[i] - 'a' + 10;
                break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                x = 16;
                r *= x;
                r += s[i] - 'A' + 10;
                break;
            default:
                r *= x;
                r += s[i] - '0';
                break;
        }
    }
    return r;
}

bool CommonObject::strToBool(const char* s) {
	if (strcmp(s, "Y") == 0) return true;
	if (strcmp(s, "y") == 0) return true;
	if (strcmp(s, "1") == 0) return true;
	return false;
}

bool CommonObject::openObjectInternal(const char* name) {
	if (contextStackP >= MAX_CONTEXT_DEPTH) return false;
	CommonObject* newContext = NULL;
	if (contextStackP > 0) {
		newContext = contextStack[contextStackP - 1].obj->createContext(name);
	} else {
		newContext = createContext(name);
	}
	if (newContext == NULL) return false;
	contextStack[contextStackP].obj   = newContext;
	contextStack[contextStackP].scope = newContext->getScope();
	contextStackP++;
	return true;
}

bool CommonObject::closeObjectInternal() {
	if (contextStackP <= 0) return false;
	contextStackP--;
	if (contextStackP > 0) {
		contextStack[contextStackP - 1].obj->closeContext(contextStack[contextStackP - 1].obj->getScope());
		contextStack[contextStackP - 1].obj->setScope(contextStack[contextStackP - 1].scope);
	}
	return true;
}

bool CommonObject::setValueInternal(const char* name, const char* value) {
	if (contextStackP > 0) {
		return contextStack[contextStackP - 1].obj->setValue(name, value);
	} else {
		return setValue(name, value);
	}
}
