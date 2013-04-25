#include "Scaner.h"

namespace YAML {

#define SPACE                          ' '
#define CR                             0x0d
#define LF                             0x0a
#define HYPHEN                         '-'
#define COLON                          ':'
#define COMMA                          ','
#define LIST_OPEN                      '['
#define LIST_CLOSE                     ']'
#define HASH_OPEN                      '{'
#define HASH_CLOSE                     '}'
#define OCTOTHORPE                     '#'
#define BANG                           '!'
#define QUESTION                       '?'
#define AMPERSAND                      '&'
#define ALIAS                          '*'

int Scaner::scan(char c) {
    switch (c) {
        case SPACE:

            break;
        default:

            break;
    }
    return PARSER_RETURN_OK;
}

}