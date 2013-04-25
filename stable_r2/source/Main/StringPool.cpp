#include "StringPool.h"
#include <string.h>

StringPool  pool;
char        data[STRING_POOL_SIZE];
const char* strs[MAX_STR_CNT];

const char* StringPool::string(const char* s) {
	for (int i = 0; i < strCnt; i++) {
		if (strcmp(s, strs[i]) == 0) {
			return strs[i];
		}
	}
	if (strCnt >= MAX_STR_CNT) return NULL;
	size_t l = strlen(s) + 1;
	if (currPos + l >= STRING_POOL_SIZE) return NULL;
	const char* r = data + currPos;
	memset(data + currPos, 0, l);
	strcpy(data + currPos, s);
	currPos += l; 
	strs[strCnt++] = r;
	return r;
}
