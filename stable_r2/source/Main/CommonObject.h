#ifndef _COMMON_OBJECT_H_
#define _COMMON_OBJECT_H_

#include "s3e.h"
#include "IObject.h"

#define MAX_CONTEXT_DEPTH       20

class CommonObject: public IObject {
	private:
		int                     scope;
	protected:
        int                     strToNum(const char* s);
        bool                    strToBool(const char* s);
		virtual CommonObject*   createContext(const char* name) {return this;}
		virtual bool            closeContext(int scope) {return true;}
		virtual bool            openObjectInternal(const char* name);
		virtual bool            closeObjectInternal();
		virtual bool            setValueInternal(const char* name, const char* value);

	public:
		CommonObject():         scope(0) {}
		virtual int             getScope() const {return scope;}
		virtual void            setScope(int s) {scope = s;}
		virtual bool            setValue(const char* name, const char* value) {return false;}
};

struct SContext {
	SContext(CommonObject* o = NULL, int s = 0): obj(o), scope(s) {}
	SContext(const SContext& p): obj(p.obj), scope(p.scope) {}
	CommonObject*               obj;
	int                         scope;
};

#endif	// _COMMON_OBJECT_H_
