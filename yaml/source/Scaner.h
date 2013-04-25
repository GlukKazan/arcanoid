#ifndef _YAML_SCANER_H_
#define _YAML_SCANER_H_

#include "Parser.h"

namespace YAML {

class Scaner {
    private:
        int state;
        Parser& p;
        int scan(char c);
    public:
        Scaner(Parser& p): p(p), state(0) {}

    friend class Parser;
};

}

#endif	// _YAML_SCANER_H_
