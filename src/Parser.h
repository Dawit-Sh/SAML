/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#ifndef PARSER_H
#define PARSER_H

#include "SAML.h"

class LexError {
public:
    LexError() {
        std::cerr << "Fatal Error! - Token stream not properly scanned!\n";
    }
};

#endif // PARSER_H
