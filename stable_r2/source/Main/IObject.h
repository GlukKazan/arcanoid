#ifndef _I_OBJECT_H_
#define _I_OBJECT_H_

class IObject {
	protected:
		virtual ~IObject() {}
		virtual bool openObjectInternal(const char* name) = 0;
		virtual bool closeObjectInternal() = 0;
		virtual bool setValueInternal(const char* name, const char* value) = 0;
	public:
		virtual int getScope() const = 0;
		virtual void setScope(int s) = 0;
		virtual void clear() {}
		virtual bool setValue(const char* name, const char* value) = 0;
		virtual bool isValid() const {return true;}

    friend class DataLoader;
};

#endif	// _I_OBJECT_H_
