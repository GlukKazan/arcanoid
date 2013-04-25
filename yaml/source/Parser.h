#ifndef _YAML_PARSER_H_
#define _YAML_PARSER_H_

enum {
    PARSER_RETURN_OK       = 0,
    PARSER_RETURN_FAIL     = 1
};

namespace YAML {

class Parser {
    private:
        void parse(int type, const char* value);
    public:
        int  load(const char* data);

    friend class Scaner;
};

}

#endif	// _YAML_PARSER_H_
