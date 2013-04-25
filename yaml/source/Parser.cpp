#include "Parser.h"
#include "Scaner.h"
#include <string.h>

namespace YAML {

int Parser::load(const char* data) {
    Scaner s(*this);
    for (size_t i = 0; i < strlen(data); i++) {
        int r = s.scan(data[i]);
        if (r != PARSER_RETURN_OK) return r;
    }
    return PARSER_RETURN_OK;
}

void Parser::parse(int type, const char* value) {
    // TODO:

}

}