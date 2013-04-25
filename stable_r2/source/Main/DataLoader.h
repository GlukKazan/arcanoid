#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

#include <yaml.h>
#include "IObject.h"

#define MAX_NAME_SZ           80

class DataLoader {
	private:
        yaml_parser_t         parser;
        yaml_event_t          event;
		char                  currentName[MAX_NAME_SZ];
		void                  clearCurrentName();
        bool                  notify(IObject* receiver);
	public:
		bool                  load(const char* name, IObject* receiver);
};

#endif	// _DATA_LOADER_H_
