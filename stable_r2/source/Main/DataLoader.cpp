#include "DataLoader.h"
#include <string.h>

void DataLoader::clearCurrentName() {
	memset(currentName, 0, sizeof(currentName));
}

bool DataLoader::load(const char* name, IObject* receiver) {
	bool r = true;
	clearCurrentName();
    yaml_parser_initialize(&parser);
    FILE *input = fopen(name, "rb");
    yaml_parser_set_input_file(&parser, input);
    int done = 0;
    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            r = false;
        }
        if (!notify(receiver)) {
			r = false;
		}
		if (!r) break;
        done = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }
    yaml_parser_delete(&parser);
    fclose(input);
	return r;
}

bool DataLoader::notify(IObject* receiver) {
	bool r = true;
    switch (event.type) {
        case YAML_MAPPING_START_EVENT:
        case YAML_SEQUENCE_START_EVENT:
			r = receiver->openObjectInternal(currentName);
			clearCurrentName();
			break;
        case YAML_MAPPING_END_EVENT:
        case YAML_SEQUENCE_END_EVENT:
			if (!receiver->closeObjectInternal()) return false;
            break;
        case YAML_SCALAR_EVENT:
			if (currentName[0] == 0) {
				strncpy(currentName, (const char*)event.data.scalar.value, sizeof(currentName) - 1);
				break;
			}
			r = receiver->setValueInternal(currentName, (const char*)event.data.scalar.value);
			clearCurrentName();
            break; 
        default:
            break;
    }
	return r;
}
