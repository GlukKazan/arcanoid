#ifndef _STRING_POOL_H_
#define _STRING_POOL_H_

#define STRING_POOL_SIZE 1024
#define MAX_STR_CNT      300

class StringPool {
	private:
		int currPos;
		int strCnt;
	public:
		StringPool(): currPos(0), strCnt(0) {}
		const char* string(const char* s);
		const char* empty() const {return "";}
};

extern StringPool pool;

#endif	// _STRING_POOL_H_
